#ifndef VALIDATEDELETE_HPP
#define VALIDATEDELETE_HPP

#include "http/states/validateRequest/ValidateRequest.hpp"
#include "utils/logger/Logger.hpp"
#include <utils/state/IState.hpp>

class ValidateDelete : public IState<ValidateRequest>
{

public:
  explicit ValidateDelete(ValidateRequest* context);
  void run();

private:
  Client* _client;
  static Logger& _log;
};

#endif