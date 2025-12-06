#pragma once
#ifndef WRITE_BODY_HPP
#define WRITE_BODY_HPP

#include <utils/buffer/SmartBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstddef>

class Client;

/* ************************************************************************** */
class WriteBody : public IState<Client>
{
public:
  explicit WriteBody(Client* context);

  void run();

private:
  void _defineBodyFraming();
  void _handleFixedLengthBody();
  void _handleChunkedBody();

  static Logger& _log;
  static const std::size_t _outBufferLimit = 3072;
  static const std::size_t _chunkSize = 1024;
  Client* _client;

  bool _done;
  bool _chunked;
  bool _fixedLength;
  SmartBuffer* _smartBuffer; // Owner in Client::_outBuffQueue
};

#endif
