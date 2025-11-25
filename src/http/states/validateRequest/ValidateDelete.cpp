#include "ValidateDelete.hpp"
#include "client/Client.hpp"
#include "config/FileUtils.hpp"
#include "http/Resource.hpp"
#include "http/StatusCode.hpp"
#include "http/states/validateRequest/ValidateRequest.hpp"
#include "utils/logger/Logger.hpp"
#include "utils/state/IState.hpp"
#include <string>

/* ************************************************************************** */
// INIT

Logger& ValidateRequest::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ValidateDelete::ValidateDelete(ValidateRequest* context)
  : IState<ValidateRequest>(context)
  , _client(context->getContext())
  , _path(context->getPath())
  , _server(context->getServer())     // TODO: not used atm
  , _location(context->getLocation()) // TODO: not used atm
{
  _log.info() << "Validate DELETE\n";
}

void ValidateDelete::run()
{
  validate();
  getContext()->getStateHandler().setDone();
}

void ValidateDelete::validate()
{
  if (config::fileutils::isDirectory(_path)) {
    // 403 most servers dont allow deleting directories
    // some allow deleting empty directories...
    endState(StatusCode::Forbidden);
    return;
  }
  if (!config::fileutils::isFile(_path)) {
    endState(StatusCode::NotFound);
    return;
  }
  validateParentDirPermissions();
}

void ValidateDelete::validateParentDirPermissions()
{
  std::string dirPath = _path.substr(0, _path.find_last_of('/'));
  if (dirPath.empty()) {
    dirPath = "/";
  }
  if (!config::fileutils::isDirectory(dirPath)) {
    endState(StatusCode::NotFound);
    return;
  }
  if (!config::fileutils::isExecuteable(dirPath)) {
    endState(StatusCode::NotFound);
    return;
  }
  if (!config::fileutils::isWriteable(dirPath)) {
    endState(StatusCode::Forbidden);
    return;
  }
}

void ValidateDelete::endState(StatusCode::Code status)
{
  _client->getResponse().setStatusCode(status);
  if (status != StatusCode::Ok) {
    _client->getResource().setType(Resource::Error);
  }
}

/*
  204 No Content if deleted successfully
  404 Not Found if the file doesn’t exist
  403 Forbidden if not allowed
  409 Conflict if the directory isn’t empty (optional)
  500 Internal Server Error if deletion fails unexpectedly
*/
