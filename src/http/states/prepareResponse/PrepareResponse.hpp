#pragma once
#ifndef PREPARE_RESPONSE_HPP
#define PREPARE_RESPONSE_HPP

#include "utils/state/StateHandler.hpp"
#include <utils/state/IState.hpp>

class Logger;
class Client;

/* ************************************************************************** */
class PrepareResponse : public IState<Client>
{
public:
  explicit PrepareResponse(Client* context);

  void run();

  StateHandler<PrepareResponse>& getStateHandler();

private:
  void _init();

  Client* _client;
  Logger* _log;
  StateHandler<PrepareResponse> _stateHandler;
  bool _initialized;
};

#endif
