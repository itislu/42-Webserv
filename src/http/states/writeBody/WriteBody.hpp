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
  void _writeIntoOutBuffer();

  static Logger& _log;
  static const int _outBufferLimit = 4096;
  static const int _chunkSize = 1024;
  Client* _client;

  bool _done;
};

#endif
