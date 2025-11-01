#pragma once
#ifndef WRITE_BODY_HPP
#define WRITE_BODY_HPP

#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

class Client;

/* ************************************************************************** */
class WriteBody : public IState<Client>
{
public:
  explicit WriteBody(Client* context);

  void run();

private:
  Client* _client;
  Logger& _log; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

#endif
