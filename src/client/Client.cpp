#include "Client.hpp"
#include "socket/AutoFd.hpp"
#include "utils/Buffer.hpp"
#include <string>

Client::Client()
  : _fd(-1)
//, _state(0)
{
}

Client::Client(int sockFd)
  : _fd(sockFd)
//, _state(0)
{
}

int Client::getFd() const
{
  return _fd.get();
}

const std::string& Client::getHost() const
{
  return _host;
}

Buffer Client::getInBuff() const
{
  return _inBuff;
}

Buffer Client::getOutBuff() const
{
  return _outBuff;
}

bool Client::hasDataToSend() const
{
  return !_outBuff.isEmpty();
}
