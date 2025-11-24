
#include "ValidateGet.hpp"
#include "client/Client.hpp"
#include "config/FileUtils.hpp"
#include "http/Resource.hpp"
#include "http/StatusCode.hpp"
#include "http/states/validateRequest/ValidateRequest.hpp"
#include "libftpp/utility.hpp"
#include "utils/logger/Logger.hpp"
#include "utils/state/IState.hpp"
#include <string>

/* ************************************************************************** */
// INIT

Logger& ValidateRequest::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ValidateGet::ValidateGet(ValidateRequest* context)
  : IState<ValidateRequest>(context)
  , _client(context->getContext())
  , _path(context->getPath())
  , _server(context->getServer())
  , _location(context->getLocation())
{
  _log.info() << "Validate GET\n";
}

void ValidateGet::run()
{
  validate();
  getContext()->getStateHandler().setDone();
}

void ValidateGet::validate()
{
  if (config::fileutils::isFile(_path)) {
    validateFile();
    return;
  }
  if (config::fileutils::isDirectory(_path)) {
    validateDirectory();
    return;
  }
  endState(StatusCode::NotFound);
}

void ValidateGet::validateFile()
{
  if (!config::fileutils::isReadable(_path)) {
    endState(StatusCode::Forbidden);
    return;
  }

  if (_location != FT_NULLPTR && _location->isCgi()) {
    // check file extension if file is cgi
    // check if parent directory is executable
    if (!config::fileutils::isExecuteable(_path)) {
      endState(StatusCode::Forbidden);
      return;
    }
    endState(StatusCode::Ok);
    return;
  }
  endState(StatusCode::Ok);
}

void ValidateGet::validateDirectory()
{
  if (_location != FT_NULLPTR && _location->isCgi()) {
    endState(StatusCode::Forbidden);
    return;
  }

  if (!config::fileutils::isExecuteable(_path) ||
      !!config::fileutils::isReadable(_path)) {
    endState(StatusCode::Forbidden);
    return;
  }

  std::string indexName;
  if (_location != FT_NULLPTR) {
    indexName = _location->getIndex();
  } else {
    indexName = _server->getIndex();
  }

  // TODO: check double slashes //
  const std::string indexPath = _path + indexName;

  if (config::fileutils::isFile(indexPath) &&
      config::fileutils::isReadable(indexPath)) {
    _client->getResource().setPath(indexPath);
    endState(StatusCode::Ok);
    return;
  }

  if (_location != FT_NULLPTR && _location->isAutoIndex()) {
    endState(StatusCode::Ok);
    return;
  }

  // No index, no autoindex → 404
  endState(StatusCode::NotFound);
}

void ValidateGet::endState(StatusCode::Code status)
{
  _client->getResponse().setStatusCode(status);
  _client->getResource().setType(Resource::Error);
}
