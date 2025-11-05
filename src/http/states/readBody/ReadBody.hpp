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
  void _readFixedLengthBody();
  void _readChunkedBody();

  Client* _client;
  Logger* _log;
  bool _initialized;
  bool _fixedLengthBody;
  std::size_t _bodyLength;
};

#endif
