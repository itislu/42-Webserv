#include "ValidateDelete.hpp"

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
