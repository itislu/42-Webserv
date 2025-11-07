#include "HandleGet.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/writeStatusLine/WriteStatusLine.hpp>
#include <libftpp/algorithm.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <fstream>
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

  std::stringstream iss;
  iss << info.st_size;
  const std::string contentLength = iss.str();
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
