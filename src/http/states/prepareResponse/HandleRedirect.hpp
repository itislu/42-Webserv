#pragma once
#ifndef HANDLE_REDIRECT_HPP
#define HANDLE_REDIRECT_HPP

#include <utils/state/IState.hpp>

class Logger;
class Client;
class PrepareResponse;

/* ************************************************************************** */
class HandleRedirect : public IState<PrepareResponse>
{
public:
  explicit HandleRedirect(PrepareResponse* context);

  void run();

private:
  PrepareResponse* _prepareResponse;
  Client* _client;
  static Logger& _log;
};

#endif
