#include "ValidateDelete.hpp"
#include "libftpp/string.hpp"
#include "libftpp/utility.hpp"

#include <client/Client.hpp>
#include <http/Resource.hpp>
#include <http/StatusCode.hpp>
#include <http/states/validateRequest/ValidateRequest.hpp>
#include <utils/fileUtils.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <string>

/* ************************************************************************** */
// INIT

Logger& ValidateDelete::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ValidateDelete::ValidateDelete(ValidateRequest* context)
  : IState<ValidateRequest>(context)
  , _client(context->getContext())
  , _path(context->getPath())
  , _location(context->getLocation())
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
  if (_location != FT_NULLPTR && _location->isCgi()) {
    const std::string& ext = _location->getCgiExtension();
    if (!ext.empty() && ft::ends_with(_path, ext)) {
      validateCGI();
      return;
    }
  }

  validateStaticDelete();
}

void ValidateDelete::validateCGI()
{
  _client->getResource().setType(Resource::Cgi);

  if (isDirectory(_path)) {
    endState(StatusCode::Forbidden);
    return;
  }
  if (!isFile(_path)) {
    endState(StatusCode::NotFound);
    return;
  }
  if (!isExecuteable(_path)) {
    endState(StatusCode::Forbidden);
    return;
  }
  endState(StatusCode::Ok);
}

void ValidateDelete::validateStaticDelete()
{
  if (isDirectory(_path)) {
    // 403: Most servers don't allow deleting directories via HTTP
    endState(StatusCode::Forbidden);
    return;
  }
  if (!isFile(_path)) {
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
  if (!isDirectory(dirPath)) {
    endState(StatusCode::NotFound);
    return;
  }
  if (!isExecuteable(dirPath)) {
    endState(StatusCode::NotFound);
    return;
  }
  if (!isWriteable(dirPath)) {
    endState(StatusCode::Forbidden);
    return;
  }
  endState(StatusCode::Ok);
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
