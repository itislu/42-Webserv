#pragma once
#ifndef WRITE_BODY_HPP
#define WRITE_BODY_HPP

#include <utils/buffer/SmartBuffer.hpp>
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
  void _defineBodyFraming();
  void _handleFixedLengthBody();
  void _handleChunkedBody();
  void _handleLastChunk();

  static Logger& _log;

  Client* _client;
  SmartBuffer* _outBuffer; // Owner in Client::_outBuffQueue
  bool _done;
  bool _chunked;
  bool _fixedLength;
};

#endif
