#include "ValidateRequest.hpp"
#include "client/Client.hpp"
#include "config/LocationConfig.hpp"
#include "http/Request.hpp"
#include "http/StatusCode.hpp"
#include "http/states/prepareResponse/PrepareResponse.hpp"
#include "http/states/readBody/ReadBody.hpp"
#include "http/states/validateRequest/ValidateDelete.hpp"
#include "http/states/validateRequest/ValidateGet.hpp"
#include "http/states/validateRequest/ValidatePost.hpp"
#include "libftpp/string.hpp"
#include "libftpp/utility.hpp"
#include "server/Server.hpp"

#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <iostream>
#include <set>
#include <string>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

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
    if (getContext()->getResponse().getStatusCode() == StatusCode::Ok) {
      getContext()->getStateHandler().setState<ReadBody>();
    } else {
      // TODO: set errorpage here?
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
  _initConfigs();
  const std::set<std::string>& allowedMethods =
    _location != FT_NULLPTR ? _location->getAllowedMethods()
                            : _server->getAllowedMethods();
  const Request::Method method = _client->getRequest().getMethod();
  if (validateMethod(allowedMethods, method)) {
    _initState(method);
    _initRequestPath(); // TODO: check this again, should the path only append,
                        // if the method is allowed?
  } else {
    _stateHandler.setDone();
    getContext()->getResponse().setStatusCode(StatusCode::MethodNotAllowed);
  }
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
  /* TODO: validate path security ../../path */
  if (_location != FT_NULLPTR) {
    _path = removePrefix(_path, _location->getPath());
    _path = appendToRoot(_path, _location->getRoot());
  } else {
    _path = appendToRoot(_path, _server->getRoot());
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
