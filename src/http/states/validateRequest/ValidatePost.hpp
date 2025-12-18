#ifndef VALIDATEPOST_HPP
#define VALIDATEPOST_HPP

#include "config/LocationConfig.hpp"
#include "http/StatusCode.hpp"
#include "http/states/validateRequest/ValidateRequest.hpp"
#include "utils/logger/Logger.hpp"
#include "utils/state/IState.hpp"
#include <string>

class ValidatePost : public IState<ValidateRequest>
{
public:
  explicit ValidatePost(ValidateRequest* context);
  void run();

private:
  void validate();
  void endState(StatusCode::Code status);

  static Logger& _log;
  Client* _client;
  std::string _path;
  const ServerConfig* _server;
  const LocationConfig* _location;
};

#endif
