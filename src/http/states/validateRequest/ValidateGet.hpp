#ifndef VALIDATEGET_HPP
#define VALIDATEGET_HPP

#include "config/LocationConfig.hpp"
#include "http/states/validateRequest/ValidateRequest.hpp"
#include "utils/logger/Logger.hpp"
#include <string>
#include <utils/state/IState.hpp>

class ValidateGet : public IState<ValidateRequest>
{

public:
  explicit ValidateGet(ValidateRequest* context);
  void run();

private:
  Client* _client;
  static Logger& _log;
  std::string _path;
  config::LocationConfig* location;
  config::ServerConfig* server;
};

#endif