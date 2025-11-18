#include "ValidateRequest.hpp"
#include "client/Client.hpp"
#include "config/FileUtils.hpp"
#include "config/LocationConfig.hpp"
#include "http/Request.hpp"
#include "libftpp/string.hpp"
#include "libftpp/utility.hpp"
#include "server/Server.hpp"

#include <cstddef>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <string>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>
#include <vector>

/* ************************************************************************** */
// INIT

Logger& ValidateRequest::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ValidateRequest::ValidateRequest(ReadRequestLine* context)
  : IState<ReadRequestLine>(context)
  , _client(context->getContext())
{
  _log.info() << "ValidateRequest\n";
}

// TODO:
// after finding location block check if method is allowed - else 405
//  lookup if file exists - else 404
//  look if file has permissions - else 403
//  check if file is file - yes, serve it
//  check if file is a directory
//     - check redirections - 30x
//     - check autoindex
//     - check check cgi
//     - else check if index.html is present - else 403
void ValidateRequest::run()
{
  if (!_client->hasServer()) {
    /*
    TODO:
    - get host from Request
    - host should be in Uri -> authority -> getHost()
    - look up servers from client socket and find matching servername -> host
    - if no exact match, use default server for that socket
    - set server in client
    */
  }

  const config::ServerConfig& serverConfig = _client->getServer()->getConfig();
  const std::string& uriPath = _client->getRequest().getUri().getPath();

  const config::LocationConfig* const location =
    serverConfig.getBestMatchLocation(uriPath);

  if (!validateMethod(location->getAllowedMethods(),
                      _client->getRequest().getMethod())) {
    /* return 405 - Method not allowed */
  }

  std::string requestPath;
  if (location == FT_NULLPTR) {
    requestPath = getRequestPath(uriPath, serverConfig.getRoot());
  } else {
    requestPath = getRequestPath(uriPath, location->getRoot());
  }

  int error = validateFile(requestPath);

  // End State
  getContext()->getStateHandler().setDone();
}

/* ************************************************************************** */
// PRIVATE

std::string ValidateRequest::getRequestPath(const std::string& uri,
                                            const std::string& root)
{
  /* TODO: what do with paths that contain .. or . */
  const bool rootHasSlash = !ft::ends_with(root, '/');
  const bool uriHasSlash = ft::starts_with(uri, '/');

  if (rootHasSlash && uriHasSlash) {
    return root + uri.substr(1);
  }
  if (rootHasSlash != uriHasSlash) {
    return root + uri;
  }
  return root + "/" + uri;
}

/* TODO: maybe reverse it so i get the string to the method and then compare
 * strings? */
bool ValidateRequest::validateMethod(
  const std::vector<std::string>& allowedMethods,
  Request::Method method)
{
  for (std::size_t i = 0; i < allowedMethods.size(); ++i) {
    if (method == Request::strToMethod(allowedMethods[i])) {
      return true;
    }
  }
  return false;
}

//  look if file has permissions - else 403
//  check if file is file - yes, serve it
//  check if file is a directory
//     - check redirections - 30x
//     - check autoindex
//     - check check cgi
//     - else check if index.html is present - else 403

int ValidateRequest::validateFile(const std::string& path)
{
  (void)path;
  return -1;
}

int ValidateRequest::validateDirectory(const std::string& path)
{
  (void)path;
  return -1;
}

int ValidateRequest::validateRequestPath(const std::string& path)
{
  if (config::fileutils::isFile(path)) {
    return validateFile(path);
  }

  if (config::fileutils::isDirectory(path)) {
    return validateDirectory(path);
  }

  //  lookup if file/directory exists - else 404

  return 404;
}

/* FILEULTIS */

static bool isExecutable(const std::string& path)
{
  return (access(path.c_str(), X_OK) == 0);
}

static bool isReadable(const std::string& path)
{
  return (access(path.c_str(), R_OK) == 0);
}