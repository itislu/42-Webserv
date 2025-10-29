#pragma once
#ifndef WRITE_STATUS_LINE_HPP
#define WRITE_STATUS_LINE_HPP

#include "client/Client.hpp"
#include "utils/state/IState.hpp"

class WriteStatusLine : public IState<Client>
{
public:
  explicit WriteStatusLine(Client* context);

  void run();

private:
  Client* _client;
};

#endif
