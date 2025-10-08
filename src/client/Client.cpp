#include "Client.hpp"
#include "utils/Buffer.hpp"

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
  return _fd;
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
