#ifndef VALIDATEDELETE_HPP
#define VALIDATEDELETE_HPP

#include "http/StatusCode.hpp"
#include "http/states/validateRequest/ValidateRequest.hpp"
#include "utils/logger/Logger.hpp"
#include <string>
#include <utils/state/IState.hpp>

class ValidateDelete : public IState<ValidateRequest>
{

public:
  explicit ValidateDelete(ValidateRequest* context);
  void run();

private:
  void validate();
  void validateParentDirPermissions();
  void endState(StatusCode::Code);

  Client* _client;
  static Logger& _log;
  std::string _path;
};

#endif
