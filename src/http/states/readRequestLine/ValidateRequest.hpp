#pragma once
#ifndef VALIDATE_REQUEST_HPP
#define VALIDATE_REQUEST_HPP

#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

class Client;
class ReadRequestLine;

/* ************************************************************************** */
class ValidateRequest : public IState<ReadRequestLine>
{
public:
  explicit ValidateRequest(ReadRequestLine* context);

  void run();

private:
  Client* _client;
  static Logger& _log;
};

#endif
