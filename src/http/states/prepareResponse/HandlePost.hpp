#pragma once
#ifndef HANDLE_POST_HPP
#define HANDLE_POST_HPP

#include "utils/state/IState.hpp"

class Logger;
class Client;
class PrepareResponse;

/* ************************************************************************** */
class HandlePost : public IState<PrepareResponse>
{
public:
  explicit HandlePost(PrepareResponse* context);

  void run();

private:
  PrepareResponse* _prepareResponse;
  Client* _client;
  Logger* _log;
};

#endif
