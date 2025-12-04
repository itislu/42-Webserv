#include "HandleGet.hpp"
#include "libftpp/memory.hpp"
#include "libftpp/utility.hpp"
#include "utils/buffer/StaticFileBuffer.hpp"

#include <client/Client.hpp>
#include <cstring>
#include <dirent.h>
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
  _client->getResponse().setBody(ft::make_shared<StaticFileBuffer>(filepath));
}

bool HandleGet::_fail()
{
  return _client->getResponse().getStatusCode() != StatusCode::Ok;
}

/* Generates a HTML for the autoindex setting in a location if no index.html is
  present. If the entry in the folder is directory it generates a clickable
  link. If the entry is a regular file it just lists the name.

  TODO: change string conncatination to .append() - for faster performance
*/
std::string HandleGet::_generateAutoindex(const std::string& path)
{
  DIR* const dir = opendir(path.c_str());
  if (dir == FT_NULLPTR) {
    return "<html><body><h1>403 Forbidden</h1></body></html>\n";
  }

  std::string html;
  html += "<html><head><title>Index of " + path + "</title></head><body>\n";
  html += "<h1>Index of " + path + "</h1>\n<ul>\n";

  struct dirent* entry = 0;
  while ((entry = readdir(dir)) != 0) {
    const char* const name = static_cast<char*>(entry->d_name);
    if (std::strcmp(name, ".") == 0 || std::strcmp(name, "..") == 0) {
      continue;
    }

    const std::string fullPath = path + "/" + name;
    if (isDirectory(fullPath)) {
      html += "<li><a href=\"";
      html += name;
      html += "/\">";
      html += name;
      html += "/</a></li>\n";
    } else {
      html += "<li>";
      html += name;
      html += "</li>\n";
    }
  }

  html += "</ul></body></html>\n";
  closedir(dir);
  return html;
}
