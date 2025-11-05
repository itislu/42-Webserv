#pragma once
#ifndef HANDLE_DELETE_HPP
#define HANDLE_DELETE_HPP

#include "utils/state/IState.hpp"

class Logger;
class Client;
class PrepareResponse;

/* ************************************************************************** */
class HandleDelete : public IState<PrepareResponse>
{
public:
  explicit HandleDelete(PrepareResponse* context);

  void run();

private:
  PrepareResponse* _prepareResponse;
  Client* _client;
  Logger* _log;
};

#endif
