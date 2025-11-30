#include "HandleGet.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Resource.hpp>
#include <http/StatusCode.hpp>
#include <http/http.hpp>
#include <http/states/prepareResponse/HandleError.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/writeStatusLine/WriteStatusLine.hpp>
#include <libftpp/optional.hpp>
#include <libftpp/string.hpp>
#include <utils/fileUtils.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstddef>
#include <fstream>
#include <string>

/* ************************************************************************** */
// INIT

Logger& HandleGet::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

HandleGet::HandleGet(PrepareResponse* context)
  : IState<PrepareResponse>(context)
  , _prepareResponse(context)
  , _client(_prepareResponse->getContext())
{
  _log.info() << "HandleGet\n";
  _log.info() << "HandleGet: " << _client->getResource().getPath() << "\n";
}

void HandleGet::run()
{
  if (_client->getResource().getType() == Resource::Autoindex) {

  } else {
    _addContentLengthHeader();
    if (!_fail()) {
      _addContentType();
    }
    if (!_fail()) {
      _openFile();
    }
  }
  _setNextState();
}

/* ************************************************************************** */
// PRIVATE

void HandleGet::_setNextState()
{
  const StatusCode& statusCode = _client->getResponse().getStatusCode();
  if (statusCode == StatusCode::Ok) {
    getContext()->getStateHandler().setDone();
  } else {
    getContext()->getStateHandler().setState<HandleError>();
  }
}

void HandleGet::_addContentLengthHeader()
{
  const std::string& filePath = _client->getResource().getPath();
  const ft::optional<std::size_t> optSize = getFileSize(filePath);
  if (!optSize.has_value()) {
    _client->getResponse().setStatusCode(StatusCode::InternalServerError);
    _log.error() << "HandleGet: failed to determine content length\n";
    return;
  }

  const std::string sizeStr = ft::to_string(*optSize);
  Headers& headers = _client->getResponse().getHeaders();
  headers.addHeader("Content-Length", sizeStr);
}

void HandleGet::_addContentType()
{
  const std::string& filePath = _client->getResource().getPath();
  const std::string fileExt = getFileExtension(filePath);
  const http::ExtToTypeMap& extToType = http::getExtToType();
  const http::ExtToTypeMap::const_iterator type = extToType.find(fileExt);

  Headers& headers = _client->getResponse().getHeaders();
  if (type != extToType.end()) {
    headers.addHeader("Content-Type", type->second);
  } else {
    headers.addHeader("Content-Type", "text/html");
  }
}

void HandleGet::_openFile()
{
  const std::string& filepath = _client->getResource().getPath();
  std::ifstream& body = _client->getResponse().getBody();
  body.open(filepath.c_str());
  if (!body.is_open()) {
    _client->getResponse().setStatusCode(StatusCode::InternalServerError);
    _log.error() << "HandleGet: failed to open file\n";
  }
}

bool HandleGet::_fail()
{
  return _client->getResponse().getStatusCode() != StatusCode::Ok;
}
