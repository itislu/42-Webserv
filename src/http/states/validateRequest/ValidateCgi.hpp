#ifndef VALIDATECGI_HPP
#define VALIDATECGI_HPP

#include "http/StatusCode.hpp"
#include "http/states/validateRequest/ValidateRequest.hpp"
#include "utils/logger/Logger.hpp"
#include <string>
#include <utils/state/IState.hpp>

class ValidateCgi : public IState<ValidateRequest>
{

public:
  explicit ValidateCgi(ValidateRequest* context);
  void run();

private:
  void validate();
  void endState(StatusCode::Code);

  Client* _client;
  static Logger& _log;
  std::string _path;
};

#endif
