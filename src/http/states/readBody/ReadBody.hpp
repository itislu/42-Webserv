#pragma once
#ifndef READ_BODY_HPP
#define READ_BODY_HPP

#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstddef>

class Client;

/* ************************************************************************** */
class ReadBody : public IState<Client>
{
public:
  explicit ReadBody(Client* context);

  void run();

private:
  void _determineBodyFraming();
  void _validateContentLength();
  void _validateTransferEncoding();

  void _readFixedLengthBody();
  void _readChunkedBody();

  static Logger& _log;
  static const std::size_t _chunkSize = 1024;
  Client* _client;
  bool _initialized;
  bool _fixedLengthBody;
  bool _chunked;
  bool _done;
  std::size_t _bodyLength;
  std::size_t _consumed;
};

#endif
