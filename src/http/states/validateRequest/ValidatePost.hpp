#ifndef VALIDATEPOST_HPP
#define VALIDATEPOST_HPP

#include "http/states/validateRequest/ValidateRequest.hpp"
#include "utils/logger/Logger.hpp"
#include <utils/state/IState.hpp>

class ValidatePost : public IState<ValidateRequest>
{
public:
  explicit ValidatePost(ValidateRequest* context);
  void run();

private:
  Client* _client;
  static Logger& _log;
};

#endif