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
  static Logger& _log;
  long _bodyLength;
  long _consumed;
  bool _initialized;
  bool _fixedLengthBody;
  bool _chunked;
  bool _done;
};

#endif
