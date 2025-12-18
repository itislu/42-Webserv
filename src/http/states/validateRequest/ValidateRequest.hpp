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
  const ServerConfig* getServer() const;
  const LocationConfig* getLocation() const;

  static std::string appendToRoot(const std::string& uri,
                                  const std::string& root);

  // public for testing
  static std::string removeDotSegments(const std::string& path);

  StateHandler<ValidateRequest>& getStateHandler();

private:
  void _init();
  void _initResource();
  void _initConfigs();
  void _initMethodState(const Request::Method& method);
  void _initRequestPath();

  bool _validateCgi();
  void _validateHost();
  void _splitHostHeader(const std::string& hostHeader, int& port);
  void _setPortFromUri(int& port);
  void _setServerByHost();

  static std::string removePrefix(const std::string& uriPath,
                                  const std::string& locPath);
  static bool validateMethod(const std::set<std::string>& allowedMethods,
                             Request::Method method);

  static std::string decodePath(const std::string& path,
                                bool (*wantDecode)(char));
  static bool validateChars(const std::string& path);

  static void removeLastSegment(std::string& output);
  static bool isPathRootBound(const std::string& path);

  void endState(StatusCode::Code status);

  Client* _client;
  static Logger& _log;
  StateHandler<ValidateRequest> _stateHandler;
  std::string _decoded;
  std::string _path;
  std::string _host;
  const ServerConfig* _server;
  const LocationConfig* _location;
};

#endif
