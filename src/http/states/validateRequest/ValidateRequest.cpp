#include "ValidateRequest.hpp"
#include "client/Client.hpp"
#include "config/Converters.hpp"
#include "config/LocationConfig.hpp"
#include "http/Request.hpp"
#include "http/Resource.hpp"
#include "http/StatusCode.hpp"
#include "http/http.hpp"
#include "http/states/prepareResponse/PrepareResponse.hpp"
#include "http/states/readBody/ReadBody.hpp"
#include "http/states/validateRequest/ValidateDelete.hpp"
#include "http/states/validateRequest/ValidateGet.hpp"
#include "http/states/validateRequest/ValidatePost.hpp"
#include "libftpp/algorithm.hpp"
#include "libftpp/optional.hpp"
#include "libftpp/string.hpp"
#include "libftpp/utility.hpp"
#include "server/Server.hpp"
#include "server/ServerManager.hpp"
#include "socket/Socket.hpp"
#include "socket/SocketManager.hpp"
#include "utils/state/StateHandler.hpp"

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <utils/convert.hpp>
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
    _log.info() << "ValidateRequest result\n"
                << _client->getResource().toString() << "\n";
    _log.info() << _client->getResponse().getStatusCode() << "\n";
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

StateHandler<ValidateRequest>& ValidateRequest::getStateHandler()
{
  return _stateHandler;
}

/* ************************************************************************** */
// PRIVATE

void ValidateRequest::_init()
{
  if (!_client->hasServer()) {
    _initServer();
    if (_client->getResponse().getStatusCode() != StatusCode::Ok) {
      return;
    }
  }

  _path = _client->getRequest().getUri().getPath();
  _initConfigs();
  _initResource();

  _log.info() << "Root: " << _server->getRoot() << "\n";

  const std::set<std::string>& allowedMethods =
    _location != FT_NULLPTR ? _location->getAllowedMethods()
                            : _server->getAllowedMethods();
  const Request::Method method = _client->getRequest().getMethod();
  _log.info() << _client->getRequest().getMethod() << "\n";

  if (!validateMethod(allowedMethods, method)) {
    _log.info() << "method is INVALID\n";
    endState(StatusCode::MethodNotAllowed);
  }
  _log.info() << "method is VALID\n";

  _initRequestPath();
  if (_client->getResponse().getStatusCode() == StatusCode::Ok) {
    _initState(method);
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

void ValidateRequest::_initConfigs()
{
  _server = &_client->getServer()->getConfig();
  _location = _server->getBestMatchLocation(_path);
}

void ValidateRequest::_initResource()
{
  _client->getResource().setType(Resource::File);
  _client->getResource().setServer(_server);
  _client->getResource().setLocation(_location);
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
      _stateHandler.setDone();
      break;
  }
}

static int alwaysDecode(int /*unused*/)
{
  return 1;
}

/**
 * 1. Decode unreserved characters.
 * 2. Normalize path (collapse . | .. | //).
 * 3. Decode all other characters (first decoding cannot produce more '%').
 * 4. Check for illegal characters (NUL).
 * 5. Check that path is not going out of root.
 * 6. Combine with root.
 */
void ValidateRequest::_initRequestPath()
{
  _log.info() << "init request path - path: " << _path << "\n";

  // 1. Decode unreserved characters.
  std::string decoded = decodePath(_path, http::isUnreserved);
  _log.info() << "decode unreserved - path: " << decoded << "\n";

  // 2. Normalize path (collapse . | .. | //).
  decoded = normalizePath(decoded, CapAtRoot).value();
  _log.info() << "normalizePath - path: " << decoded << "\n";

  // 3. Decode all other characters.
  decoded = decodePath(decoded, alwaysDecode);
  _log.info() << "decode all - path: " << decoded << "\n";

  // 4. Check for illegal characters (NUL).
  if (!validateChars(decoded)) {
    endState(StatusCode::BadRequest);
    return;
  }

  // 5. Check that path is not going out of root.
  if (!normalizePath(decoded, FailAboveRoot).has_value()) {
    endState(StatusCode::BadRequest);
    return;
  }

  // 6. Combine with root.
  if (_location != FT_NULLPTR) {
    _path = removePrefix(decoded, _location->getPath());
    _log.info() << "remove Prefix - path: " << _path << "\n";
    _path = appendToRoot(_path, _location->getRoot());
  } else {
    _path = appendToRoot(decoded, _server->getRoot());
  }
  _log.info() << "appendToRoot - path: " << _path << "\n";
  _client->getResource().setPath(_path);
  _log.info() << "_path: " << _path << "\n";
}

std::string ValidateRequest::decodePath(const std::string& path,
                                        int (*wantDecode)(int))
{
  std::string decoded;
  decoded.reserve(path.size());
  const int hexMult = 16;

  for (std::size_t i = 0; i < path.size(); ++i) {
    if (path[i] == '%') {
      // decode hex
      const int hex1 = utils::hexToInt(path[i + 1]);
      const int hex2 = utils::hexToInt(path[i + 2]);
      const char decode = static_cast<char>((hex1 * hexMult) + hex2);
      if (wantDecode(decode) != 0) {
        decoded += decode;
        i += 2;
        continue;
      }
    }
    decoded += path[i];
  }
  return decoded;
}

bool ValidateRequest::validateChars(const std::string& path)
{
  return !ft::contains(path, '\0');
}

ft::optional<std::string> ValidateRequest::normalizePath(
  const std::string& path,
  NormalizationMode mode)
{
  std::vector<std::string> segments;
  std::string token;
  std::stringstream stream(path);

  while (!std::getline(stream, token, '/').fail()) {
    if (token.empty() || token == ".") {
      continue;
    }
    if (token == "..") {
      if (!segments.empty()) {
        segments.pop_back();
      } else if (mode == FailAboveRoot) {
        return ft::nullopt;
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

/*
  TODO:
  - check Request - headers - and find hostname
  - get host from Request
  - host should be in Uri -> authority -> getHost() - if empty
  - look up servers from client socket and find matching servername -> host
  - if no exact match, use default server for that socket
  - set server in client
*/
void ValidateRequest::_initServer()
{
  std::string hostHeader = _client->getRequest().getHeaders().at("host");
  _log.info() << "Host Header: [" << hostHeader << "]\n";

  if (hostHeader.empty()) {
    hostHeader = _client->getRequest().getUri().getAuthority().getHost();
    _log.info() << "Uri Host: [" << hostHeader << "]\n"; // empty in 1.1
  }
  if (!hostHeader.empty()) {
    _setServerByHost(hostHeader); // TODO: set default sever - make Managers
                                  // singletons to do this
  } else {
    // TODO: Default server
  }
}

void ValidateRequest::_setServerByHost(const std::string& hostHeader)
{
  std::string host;
  int port = 0;
  _splitHostHeader(hostHeader, host, port);
  if (_client->getResponse().getStatusCode() == StatusCode::Ok) {
    _log.info() << "Split: \n";
    _log.info() << "  Host: " << host << "\n";
    _log.info() << "  Port: " << port << "\n";
  }

  const Socket& socket =
    SocketManager::getInstance().getSocketFromClientFd(_client->getFd());

  // if (socket.getPort() != port) {
  //   // Something very wrong here
  // }

  const Server* const server =
    ServerManager::getInstance().getServerByHost(&socket, host);
  if (server != FT_NULLPTR) {
    _log.info() << "Found server for host: " << host << "\n";
    _client->setServer(server);
  } else {
    _log.info() << "Found no server for host: " << host << "\n";
    // TODO: set default server
  }
}

void ValidateRequest::_splitHostHeader(const std::string& hostHeader,
                                       std::string& host,
                                       int& port)
{
  const std::size_t pos = hostHeader.find(':');
  if (pos != std::string::npos) {

    host = hostHeader.substr(0, pos);
    const std::string portStr = hostHeader.substr(pos + 1, hostHeader.size());
    if (!portStr.empty()) {
      try {
        port = config::convert::toPort(portStr);
      } catch (const std::exception& e) {
        // NOTE: not sure if this is even possible
        // Bad Request? - confirm with testing
        endState(StatusCode::BadRequest);
      }
    }
  } else {
    host = hostHeader;
  }
}
