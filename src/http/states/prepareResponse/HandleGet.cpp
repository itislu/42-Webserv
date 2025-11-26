#include "HandleGet.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/StatusCode.hpp>
#include <http/http.hpp>
#include <http/states/prepareResponse/HandleError.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/writeStatusLine/WriteStatusLine.hpp>
#include <libftpp/algorithm.hpp>
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
}

void HandleGet::run()
{
  _addContentLengthHeader();
  if (!_fail()) {
    _addContentType();
  }
  if (!_fail()) {
    _openFile();
  }
  getContext()->getStateHandler().setDone();
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

std::string HandleGet::_getResource()
{
  // todo get actual resource
  const std::string& path = _client->getRequest().getUri().getPath();
  if (ft::contains_subrange(path, std::string("get"))) {
    return "./assets/testWebsite/get.html";
  }
  if (ft::contains_subrange(path, std::string("post"))) {
    return "./assets/testWebsite/post.html";
  }
  if (ft::contains_subrange(path, std::string("delete"))) {
    return "./assets/testWebsite/delete.html";
  }
  return "./assets/testWebsite/index.html";
}

void HandleGet::_addContentLengthHeader()
{
  const std::string filePath = _getResource();
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
  const std::string filePath = _getResource();
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
  const std::string filepath = _getResource();
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
