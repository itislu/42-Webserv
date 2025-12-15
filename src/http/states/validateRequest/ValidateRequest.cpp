#include "ValidateRequest.hpp"
#include "config/parser/Converters.hpp"
#include "http/Headers.hpp"
#include "server/ServerManager.hpp"
#include "socket/Socket.hpp"

#include <client/Client.hpp>
#include <config/LocationConfig.hpp>
#include <http/Request.hpp>
#include <http/Resource.hpp>
#include <http/StatusCode.hpp>
#include <http/http.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/readBody/ReadBody.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <http/states/startCgi/StartCgi.hpp>
#include <http/states/validateRequest/ValidateDelete.hpp>
#include <http/states/validateRequest/ValidateGet.hpp>
#include <http/states/validateRequest/ValidatePost.hpp>
#include <libftpp/algorithm.hpp>
#include <libftpp/string.hpp>
#include <libftpp/utility.hpp>
#include <server/Server.hpp>
#include <utils/convert.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>
#include <utils/state/StateHandler.hpp>

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <set>
#include <sstream>
#include <string>

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
  _log.info() << *_client << " ValidateRequest\n";
}

void ValidateRequest::run()
try {
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
      if (_client->getResource().getType() == Resource::Cgi) {
        getContext()->getStateHandler().setState<StartCgi>();
      } else {
        getContext()->getStateHandler().setState<ReadBody>();
      }
    } else {
      getContext()->getStateHandler().setState<PrepareResponse>();
    }
  }
} catch (const std::exception& e) {
  _log.error() << *getContext() << " ValidateRequest: " << e.what() << '\n';
  getContext()->getResponse().setStatusCode(StatusCode::InternalServerError);
  getContext()->getStateHandler().setState<PrepareResponse>();
}

const std::string& ValidateRequest::getPath() const
{
  return _path;
}
const ServerConfig* ValidateRequest::getServer() const
{
  return _server;
}
const LocationConfig* ValidateRequest::getLocation() const
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
  _validateHost();
  if (_client->getResponse().getStatusCode() != StatusCode::Ok) {
    return;
  }

  if (!_client->hasServer()) {
    _setServerByHost();
  }

  _path = _client->getRequest().getUri().getPath();
  _initConfigs();
  _initResource();

  _log.info() << "Root: " << _server->getRoot() << "\n";

  const std::set<std::string>& allowedMethods =
    _location != FT_NULLPTR ? _location->getAllowedMethods()
                            : _server->getAllowedMethods();
  const Request::Method method = _client->getRequest().getMethod();

  if (!validateMethod(allowedMethods, method)) {
    _log.info() << "method is INVALID\n";
    endState(StatusCode::MethodNotAllowed);
    return;
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

static bool alwaysDecode(char /*unused*/)
{
  return true;
}

/**
 * 1. Decode unreserved characters.
 * 2. Normalize path (collapse '.' & '..').
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

  // 2. Normalize path (collapse '.' & '..').
  decoded = removeDotSegments(decoded);
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
  if (!isPathRootBound(decoded)) {
    endState(StatusCode::Forbidden);
    return;
  }

  // Store this path in resource so we can use it when generating autoindex
  // without exposing internal filesystem
  _client->getResource().setNoRootPath(decoded);

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
                                        bool (*wantDecode)(char))
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
      if (wantDecode(decode)) {
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

void ValidateRequest::removeLastSegment(std::string& output)
{
  const std::size_t lastSlash = output.rfind('/');
  if (lastSlash == std::string::npos) {
    output.clear();
  } else {
    output.erase(lastSlash);
  }
}

/**
 * https://datatracker.ietf.org/doc/html/rfc3986#section-5.2.4
 */
std::string ValidateRequest::removeDotSegments(const std::string& path)
{
  std::string input = path;
  std::string output;

  while (!input.empty()) {
    // A. If the input buffer begins with a prefix of "../" or "./",
    //    then remove that prefix from the input buffer.
    if (ft::starts_with(input, "../")) {
      input.erase(0, 3);
    } else if (ft::starts_with(input, "./")) {
      input.erase(0, 2);
    }

    // B. If the input buffer begins with a prefix of "/./" or "/.",
    //    where "." is a complete path segment, then replace that prefix with
    //    "/"
    else if (ft::starts_with(input, "/./")) {
      input.replace(0, 3, "/");
    } else if (input == "/.") {
      input.replace(0, 2, "/");
    }

    // C. If the input buffer begins with a prefix of "/../" or "/..",
    //    where ".." is a complete path segment, then replace that prefix with
    //    "/" and remove the last segment from the output buffer.
    else if (ft::starts_with(input, "/../")) {
      input.replace(0, 4, "/");
      removeLastSegment(output);
    } else if (input == "/..") {
      input.replace(0, 3, "/");
      removeLastSegment(output);
    }

    // D. If the input buffer consists only of "." or "..", then remove
    //    that from the input buffer.
    else if (input == "." || input == "..") {
      input.clear();
    }

    // E. Move the first path segment in the input buffer to the end of
    //    the output buffer, including the initial "/" character (if any)
    //    and any subsequent characters up to, but not including, the next "/"
    else {
      const std::size_t nextSlash = input.find('/', 1);
      if (nextSlash == std::string::npos) {
        output += input;
        input.clear();
      } else {
        output += input.substr(0, nextSlash);
        input.erase(0, nextSlash);
      }
    }
  }
  return output;
}

bool ValidateRequest::isPathRootBound(const std::string& path)
{
  std::istringstream pathStream(path);
  std::string segment;
  std::size_t depth = 0;

  while (!std::getline(pathStream, segment, '/').fail()) {
    if (segment.empty() || segment == ".") {
      continue;
    }
    if (segment == "..") {
      if (depth == 0) {
        return false;
      }
      --depth;
    } else {
      ++depth;
    }
  }
  return true;
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

void ValidateRequest::_validateHost()
{
  std::string hostHeader;
  const Headers& headers = _client->getRequest().getHeaders();

  if (headers.contains("host")) {
    hostHeader = headers.at("host");
  }

  // HTTP 1.1 MUST have a host header
  if (_client->getRequest().getVersion() == http::HTTP_1_1 &&
      hostHeader.empty()) {
    endState(StatusCode::BadRequest);
    return;
  }

  int hostPort = -1;
  _host = _client->getRequest().getUri().getAuthority().getHost();
  if (_host.empty()) {
    if (!hostHeader.empty()) {
      _splitHostHeader(hostHeader, hostPort);
    } else {
      // HTTP/1.0 with no Host-Header or URI-host
      // endState(StatusCode::BadRequest); depends on how we want to implent it
      return;
    }
  } else {
    std::string uriPort =
      _client->getRequest().getUri().getAuthority().getPort();
    if (!uriPort.empty()) {
      if (ft::starts_with(uriPort, ':')) {
        uriPort = uriPort.substr(1);
      }
      hostPort = config::convert::toPort(uriPort);
    }
  }

  const int httpPort = 80;
  const Socket* const socket = _client->getSocket();
  const int port = (hostPort == -1) ? httpPort : hostPort;
  if (socket->getPort() != port) {
    endState(StatusCode::BadRequest);
    return;
  }

  _host = ft::to_lower(_host);
}

void ValidateRequest::_splitHostHeader(const std::string& hostHeader, int& port)
{
  const std::size_t pos = hostHeader.find(':');
  if (pos != std::string::npos) {
    _host = hostHeader.substr(0, pos);
    const std::string portStr = hostHeader.substr(pos + 1, hostHeader.size());
    if (!portStr.empty()) {
      try {
        port = config::convert::toPort(portStr);
      } catch (const std::exception& e) {
        // NOTE: not sure if this is even possible
        endState(StatusCode::BadRequest);
        return;
      }
    }
  } else {
    _host = hostHeader;
  }
}

void ValidateRequest::_setServerByHost()
{
  const Socket* const socket = _client->getSocket();
  const Server* const server =
    ServerManager::getInstance().getServerByHost(socket, _host);
  _log.info() << "Found server for host: " << _host << "\n";
  _client->setServer(server);
}
