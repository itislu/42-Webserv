#include "ValidateGet.hpp"
#include "libftpp/string.hpp"

#include <client/Client.hpp>
#include <http/Resource.hpp>
#include <http/StatusCode.hpp>
#include <http/states/validateRequest/ValidateRequest.hpp>
#include <libftpp/utility.hpp>
#include <utils/fileUtils.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <string>

/* ************************************************************************** */
// INIT

Logger& ValidateGet::_log = Logger::getInstance(LOG_HTTP);

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
  if (isFile(_path)) {
    _log.info() << "is a file\n";
    validateFile();
    return;
  }
  if (isDirectory(_path)) {
    _log.info() << "is a directory\n";
    validateDirectory();
    return;
  }
  _log.info() << "path not found\n";
  endState(StatusCode::NotFound);
}

void ValidateGet::validateFile()
{
  if (!isReadable(_path)) {
    endState(StatusCode::Forbidden);
    return;
  }

  if (_location != FT_NULLPTR && _location->isCgi()) {
    const std::string& ext = _location->getCgiExtension();
    if (!ext.empty() && ft::ends_with(_path, ext)) {
      if (!isExecuteable(_path)) {
        endState(StatusCode::Forbidden);
        return;
      }
      // is CGI
      _log.info() << "Is a CGI-Script!\n";
      _client->getResource().setType(Resource::Cgi);
      endState(StatusCode::Ok);
      return;
    }
  }
  endState(StatusCode::Ok);
}

void ValidateGet::validateDirectory()
{
  // if (_location != FT_NULLPTR && _location->isCgi()) {
  //   endState(StatusCode::Forbidden);
  //   return;
  // }

  if (!isExecuteable(_path) || !isReadable(_path)) {
    endState(StatusCode::Forbidden);
    return;
  }

  std::string indexName;
  if (_location != FT_NULLPTR) {
    indexName = _location->getIndex();
  } else {
    indexName = _server->getIndex();
  }
  _log.info() << "indexName: " << indexName << "\n";
  _log.info() << "Path: " << _path << "\n";

  const std::string indexPath = ValidateRequest::appendToRoot(indexName, _path);
  _log.info() << "indexPath: " << indexPath << "\n";

  if (isFile(indexPath) && isReadable(indexPath)) {
    _client->getResource().setPath(indexPath);
    endState(StatusCode::Ok);
    return;
  }

  if (_location != FT_NULLPTR && _location->isAutoIndex()) {
    _client->getResource().setType(Resource::Autoindex);
    endState(StatusCode::Ok);
    return;
  }

  // No index, no autoindex → 404
  endState(StatusCode::NotFound);
}

void ValidateGet::endState(StatusCode::Code status)
{
  _log.info() << "endState called with status: " << status << "\n";
  _client->getResponse().setStatusCode(status);
  if (status != StatusCode::Ok) {
    _client->getResource().setType(Resource::Error);
  }
}
