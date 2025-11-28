#pragma once
#ifndef VALIDATE_REQUEST_HPP
#define VALIDATE_REQUEST_HPP

#include "config/LocationConfig.hpp"
#include "http/Request.hpp"
#include "http/StatusCode.hpp"
#include "utils/state/StateHandler.hpp"
#include <set>
#include <string>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

class Client;
class ReadRequestLine;

/* ************************************************************************** */
class ValidateRequest : public IState<Client>
{
public:
  explicit ValidateRequest(Client* context);

  void run();

  const std::string& getPath() const;
  const config::ServerConfig* getServer() const;
  const config::LocationConfig* getLocation() const;

  static std::string appendToRoot(const std::string& uri,
                                  const std::string& root);

  StateHandler<ValidateRequest>& getStateHandler();

private:
  void _init();
  void _initResource();
  void _initServer();
  void _initConfigs();
  void _initState(const Request::Method& method);
  void _initRequestPath();

  static void _splitHostHeader(const std::string& hostHeader,
                        std::string& host,
                        int& port);
  void _setServerByHost(const std::string& hostHeader);

  static std::string removePrefix(const std::string& uriPath,
                                  const std::string& locPath);
  static bool validateMethod(const std::set<std::string>& allowedMethods,
                             Request::Method method);

  static std::string decodePath(const std::string& path);
  static bool validateChars(const std::string& path);
  static std::string normalizePath(const std::string& path);

  void endState(StatusCode::Code status);

  Client* _client;
  static Logger& _log;
  StateHandler<ValidateRequest> _stateHandler;
  std::string _path;
  const config::ServerConfig* _server;
  const config::LocationConfig* _location;
};

#endif
