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
  void _setNextState();
  void _deleteFile();

  static Logger& _log;
  PrepareResponse* _prepareResponse;
  Client* _client;
};

#endif
