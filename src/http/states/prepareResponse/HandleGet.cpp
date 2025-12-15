#include "HandleGet.hpp"

#include <client/Client.hpp>
#include <http/Resource.hpp>
#include <http/Response.hpp>
#include <http/StatusCode.hpp>
#include <http/headerUtils.hpp>
#include <http/states/prepareResponse/HandleError.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/writeStatusLine/WriteStatusLine.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/buffer/SmartBuffer.hpp>
#include <utils/buffer/StaticFileBuffer.hpp>
#include <utils/fileUtils.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstring>
#include <dirent.h>
#include <exception>
#include <string>

/* ***************************************************************************/
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
    _handleAutoIndex();
  } else {
    _handleStaticFile();
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

void HandleGet::_handleAutoIndex()
{
  Response& response = _client->getResponse();
  ft::shared_ptr<SmartBuffer> buffer = ft::make_shared<SmartBuffer>();
  const Resource& resource = _client->getResource();
  _generateAutoindex(resource.getPath(), *buffer);
  response.setBody(ft::move(buffer));

  // set headers
  setContentLengthHeader(response.getHeaders(), response.getBody()->size());
  response.getHeaders().setHeader(header::contentType, "text/html");
}

void HandleGet::_handleStaticFile()
{
  Response& response = _client->getResponse();

  // set body
  const std::string& filePath = _client->getResource().getPath();
  response.setBody(ft::make_shared<StaticFileBuffer>(filePath));

  // set headers
  setContentLengthHeader(response.getHeaders(), response.getBody()->size());
  setContentTypeHeader(response.getHeaders(), filePath);
}

/*
  Generates a HTML for the autoindex setting in a location if no index.html is
  present. If the entry in the folder is directory it generates a clickable
  link. If the entry is a regular file it just lists the name.
*/
void HandleGet::_generateAutoindex(const std::string& path, SmartBuffer& html)
{
  DIR* const dir = opendir(path.c_str());
  if (dir == FT_NULLPTR) {
    _client->getResponse().setStatusCode(StatusCode::InternalServerError);
    return;
  }
  try {
    html.append("<html><head><title>Index of ");
    html.append(path);
    html.append("</title></head><body>\n");
    html.append("<h1>Index of ");
    html.append(path);
    html.append("</h1>\n<ul>\n");

    struct dirent* entry = 0;
    while ((entry = readdir(dir)) != 0) {
      const char* const name = static_cast<char*>(entry->d_name);
      if (std::strcmp(name, ".") == 0 || std::strcmp(name, "..") == 0) {
        continue;
      }

      const std::string fullPath = path + "/" + name;
      if (isDirectory(fullPath)) {
        html.append("<li><a href=\"");
        html.append(name);
        html.append("/\">");
        html.append(name);
        html.append("/</a></li>\n");
      } else {
        html.append("<li>");
        html.append(name);
        html.append("</li>\n");
      }
    }
    html.append("</ul></body></html>\n");
    closedir(dir);
  } catch (const std::exception& e) {
    closedir(dir);
    throw;
  }
}
