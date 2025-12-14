#pragma once
#ifndef HANDLE_GET_HPP
#define HANDLE_GET_HPP

#include "utils/state/IState.hpp"

class Logger;
class Client;
class PrepareResponse;

/* ************************************************************************** */
class HandleGet : public IState<PrepareResponse>
{
public:
  explicit HandleGet(PrepareResponse* context);

  void run();

private:
  void _setNextState();
  void _handleAutoIndex();
  void _handleStaticFile();

  static Logger& _log;
  PrepareResponse* _prepareResponse;
  Client* _client;
};

#endif
