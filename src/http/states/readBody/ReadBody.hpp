#pragma once
#ifndef READ_BODY_HPP
#define READ_BODY_HPP

#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

class Client;

/* ************************************************************************** */
class ReadBody : public IState<Client>
{
public:
  explicit ReadBody(Client* context);

  void run();

private:
  void _determineBodyFraming();
  bool _isValidContentLength();
  bool _isValidTransferEncoding();

  void _readFixedLengthBody();
  void _readChunkedBody();

  Client* _client;
  Logger* _log;
  bool _initialized;
  bool _fixedLengthBody;
  bool _chunked;
  long _bodyLength;
  long _consumed;
  bool _done;
};

#endif
