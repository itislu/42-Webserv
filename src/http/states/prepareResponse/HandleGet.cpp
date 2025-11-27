#include "HandleGet.hpp"
#include "config/FileUtils.hpp"
#include "libftpp/utility.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/writeStatusLine/WriteStatusLine.hpp>
#include <libftpp/algorithm.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstring>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>

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
  _addContentType();
  _openFile();
  getContext()->getStateHandler().setDone();
}

/* ************************************************************************** */
// PRIVATE

void HandleGet::_addContentLengthHeader()
{
  struct stat info = {};

  const std::string& path = _client->getRequest().getUri().getPath();

  // todo just for testing
  if (ft::contains_subrange(path, std::string("get"))) {
    stat("./assets/testWebsite/get.html", &info);
  } else if (ft::contains_subrange(path, std::string("post"))) {
    stat("./assets/testWebsite/post.html", &info);
  } else if (ft::contains_subrange(path, std::string("delete"))) {
    stat("./assets/testWebsite/delete.html", &info);
  } else {
    stat("./assets/testWebsite/index.html", &info);
  }

  std::ostringstream oss;
  oss << info.st_size;
  const std::string contentLength = oss.str();
  Headers& headers = _client->getResponse().getHeaders();
  headers.addHeader("Content-Length", contentLength);
}

void HandleGet::_addContentType()
{
  Headers& headers = _client->getResponse().getHeaders();
  headers.addHeader("Content-Type", "text/html");
}

void HandleGet::_openFile()
{
  const std::string& path = _client->getRequest().getUri().getPath();

  // todo just for testing
  std::string file;
  if (ft::contains_subrange(path, std::string("get"))) {
    file = "./assets/testWebsite/get.html";
  } else if (ft::contains_subrange(path, std::string("post"))) {
    file = "./assets/testWebsite/post.html";
  } else if (ft::contains_subrange(path, std::string("delete"))) {
    file = "./assets/testWebsite/delete.html";
  } else {
    file = "./assets/testWebsite/index.html";
  }

  std::ifstream& body = _client->getResponse().getBody();
  body.open(file.c_str());
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
    if (config::fileutils::isDirectory(fullPath)) {
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
