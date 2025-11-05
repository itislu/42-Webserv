#pragma once
#ifndef WRITE_HEADER_LINES_HPP
#define WRITE_HEADER_LINES_HPP

#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstddef>

class Client;

/* ************************************************************************** */
class WriteHeaderLines : public IState<Client>
{
public:
  explicit WriteHeaderLines(Client* context);

  void run();

private:
  Client* _client;
  Logger* _log;
};

#endif
