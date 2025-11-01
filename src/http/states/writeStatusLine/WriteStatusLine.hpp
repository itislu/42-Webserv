#pragma once
#ifndef WRITE_STATUS_LINE_HPP
#define WRITE_STATUS_LINE_HPP

#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

class Client;

/* ************************************************************************** */
class WriteStatusLine : public IState<Client>
{
public:
  explicit WriteStatusLine(Client* context);

  void run();

private:
  Client* _client;
  Logger* _log;
};

#endif
