#include "ValidateRequest.hpp"
#include "client/Client.hpp"
#include "config/Converters.hpp"
#include "config/LocationConfig.hpp"
#include "http/Request.hpp"
#include "http/Resource.hpp"
#include "http/StatusCode.hpp"
#include "http/states/prepareResponse/PrepareResponse.hpp"
#include "http/states/readBody/ReadBody.hpp"
#include "http/states/validateRequest/ValidateDelete.hpp"
#include "http/states/validateRequest/ValidateGet.hpp"
#include "http/states/validateRequest/ValidatePost.hpp"
#include "libftpp/string.hpp"
#include "libftpp/utility.hpp"
#include "server/Server.hpp"

#include <cstddef>
#include <cstdlib>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>
#include <vector>

/* ************************************************************************** */
// INIT

Logger& ValidateRequest::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ValidateRequest::ValidateRequest(Client* context)
  : IState<Client>(context)
  , _client(context)
  , _stateHandler(this)
  , _server()
  , _location()
{
  _log.info() << "ValidateRequest\n";
  std::cout << "ValidateRequest\n";
}

void ValidateRequest::run()
{
  _init();

  _stateHandler.setStateHasChanged(true);
  while (!_stateHandler.isDone() && _stateHandler.stateHasChanged()) {
    _stateHandler.setStateHasChanged(false);
    _stateHandler.getState()->run();
  }

  if (_stateHandler.isDone()) {
    if (getContext()->getResponse().getStatusCode() == StatusCode::Ok) {
      getContext()->getStateHandler().setState<ReadBody>();
    } else {
      getContext()->getStateHandler().setState<PrepareResponse>();
    }
  }
}

const std::string& ValidateRequest::getPath() const
{
  return _path;
}
const config::ServerConfig* ValidateRequest::getServer() const
{
  return _server;
}
const config::LocationConfig* ValidateRequest::getLocation() const
{
  return _location;
}

/* ************************************************************************** */
// PRIVATE

void ValidateRequest::_init()
{
  if (!_client->hasServer()) {
    _initServer();
  }
  _path = _client->getRequest().getUri().getPath();
  _client->getResource().setType(Resource::File);
  _initConfigs();
  const std::set<std::string>& allowedMethods =
    _location != FT_NULLPTR ? _location->getAllowedMethods()
                            : _server->getAllowedMethods();
  const Request::Method method = _client->getRequest().getMethod();
  if (validateMethod(allowedMethods, method)) {
    _initState(method);
    _initRequestPath();
  } else {
    endState(StatusCode::MethodNotAllowed);
  }
}

bool ValidateRequest::validateMethod(
  const std::set<std::string>& allowedMethods,
  Request::Method method)
{
  for (std::set<std::string>::const_iterator it = allowedMethods.begin();
       it != allowedMethods.end();
       ++it) {
    if (method == Request::strToMethod(*it)) {
      return true;
    }
  }
  return false;
}

void ValidateRequest::_initServer()
{
  /*
    TODO:
    - get host from Request
    - host should be in Uri -> authority -> getHost()
    - look up servers from client socket and find matching servername -> host
    - if no exact match, use default server for that socket
    - set server in client
  */
}

void ValidateRequest::_initConfigs()
{
  _server = &_client->getServer()->getConfig();
  _location = _server->getBestMatchLocation(_path);
}

void ValidateRequest::_initState(const Request::Method& method)
{
  switch (method) {
    case Request::GET:
      _stateHandler.setState<ValidateGet>();
      break;
    case Request::POST:
      _stateHandler.setState<ValidatePost>();
      break;
    case Request::DELETE:
      _stateHandler.setState<ValidateDelete>();
      break;
    case Request::UNDEFINED:
      getContext()->getStateHandler().setState<PrepareResponse>();
      break;
  }
}

void ValidateRequest::_initRequestPath()
{
  // 1. Decode URI
  std::string decoded = decodePath(_path);
  if (decoded.empty() && !_path.empty()) {
    endState(StatusCode::BadRequest);
    return;
  }

  // 2. check for illegal characters (NULL, control chars)
  if (!validateChars(decoded)) {
    endState(StatusCode::BadRequest);
    return;
  }

  // 3. Normalize Path (collapse . / .. / //)
  decoded = normalizePath(decoded);

  // 4. Combine with root
  if (_location != FT_NULLPTR) {
    _path = removePrefix(_path, _location->getPath());
    _path = appendToRoot(_path, _location->getRoot());
  } else {
    _path = appendToRoot(_path, _server->getRoot());
  }
  _client->getResource().setPath(_path);
}

std::string ValidateRequest::decodePath(std::string& path)
{
  std::string decoded;
  decoded.reserve(path.size());

  for (std::size_t i = 0; i < path.size(); ++i) {
    if (path[i] == '%') {
      // decode hex
      if (path.size() < i + 2) {
        return ""; // error
      }
      const int hex1 = config::convert::hexToInt(path[i + 1]);
      const int hex2 = config::convert::hexToInt(path[i + 2]);
      if (hex1 < 0 || hex2 < 0) {
        return ""; // error
      }
    } else {
      decoded += path[i];
    }
  }
  return decoded;
}

bool ValidateRequest::validateChars(const std::string& path)
{
  for (size_t i = 0; i < path.size(); ++i) {
    const unsigned char chr = path[i];
    if (chr < ' ') {
      return false;
    }
  }
  return true;
}

std::string ValidateRequest::normalizePath(std::string& path)
{
  std::vector<std::string> segments;
  std::string token;
  std::stringstream stream(path);

  while (std::getline(stream, token, '/') != 0) {
    if (token.empty() || token == ".") {
      continue;
    }
    if (token == "..") {
      if (!segments.empty()) {
        segments.pop_back();
      }
    } else {
      segments.push_back(token);
    }
  }

  std::string final = "/";
  for (std::size_t i = 0; i < segments.size(); ++i) {
    final += segments[i];
    if (i + 1 < segments.size()) {
      final += "/";
    }
  }
  return final;
}

std::string ValidateRequest::removePrefix(const std::string& uriPath,
                                          const std::string& locPath)
{
  if (locPath == "/") {
    return uriPath;
  }

  if (uriPath.size() == locPath.size()) {
    return "/";
  }

  return uriPath.substr(locPath.size());
}

std::string ValidateRequest::appendToRoot(const std::string& uri,
                                          const std::string& root)
{
  const bool rootHasSlash = ft::ends_with(root, '/');
  const bool uriHasSlash = ft::starts_with(uri, '/');

  if (rootHasSlash && uriHasSlash) {
    return root + uri.substr(1);
  }
  if (rootHasSlash != uriHasSlash) {
    return root + uri;
  }
  return root + "/" + uri;
}

void ValidateRequest::endState(StatusCode::Code status)
{
  _client->getResponse().setStatusCode(status);
  if (status != StatusCode::Ok) {
    _client->getResource().setType(Resource::Error);
    _stateHandler.setDone();
  }
}
