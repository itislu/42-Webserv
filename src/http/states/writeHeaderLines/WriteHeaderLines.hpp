#pragma once
#ifndef WRITE_HEADER_LINES_HPP
#define WRITE_HEADER_LINES_HPP

#include <utils/buffer/SmartBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <string>

class Client;

/* ************************************************************************** */
class WriteHeaderLines : public IState<Client>
{
public:
  explicit WriteHeaderLines(Client* context);

  void run();

private:
  static Logger& _log;
  static std::string _makeHttpDate();

  Client* _client;
  SmartBuffer* _buffer;
};

#endif
