#pragma once
#ifndef WAIT_FOR_CGI_HPP
#define WAIT_FOR_CGI_HPP

#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

class Client;

/* ************************************************************************** */
class WaitForCgi : public IState<Client>
{
public:
  explicit WaitForCgi(Client* context);

  void run();

private:
  static Logger& _log;

  Client* _client;
};

#endif
