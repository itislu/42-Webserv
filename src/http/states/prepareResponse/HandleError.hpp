#pragma once
#ifndef HANDLE_ERROR_HPP
#define HANDLE_ERROR_HPP

#include "utils/state/IState.hpp"

class Logger;
class Client;
class PrepareResponse;

/* ************************************************************************** */
class HandleError : public IState<PrepareResponse>
{
public:
  explicit HandleError(PrepareResponse* context);

  void run();

private:
  PrepareResponse* _prepareResponse;
  Client* _client;
  static Logger& _log;
};

#endif
