#pragma once
#ifndef START_CGI_HPP
#define START_CGI_HPP

#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

class Client;

/* ************************************************************************** */
class StartCgi : public IState<Client>
{
public:
  explicit StartCgi(Client* context);

  void run();

private:
  static Logger& _log;

  Client* _client;
};

#endif
