#ifndef VALIDATEGET_HPP
#define VALIDATEGET_HPP

#include "config/LocationConfig.hpp"
#include "http/StatusCode.hpp"
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
  void validate();
  void validateFile();
  void validateDirectory();
  void endState(StatusCode::Code status);

  Client* _client;
  static Logger& _log;
  std::string _path;
  const ServerConfig* _server;
  const LocationConfig* _location;
};

#endif
