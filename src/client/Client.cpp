#include "Client.hpp"
#include <string>
#include <sys/types.h>
#include <vector>

Client::Client()
  : _fd(-1)
  , _state(0)
{
}

Client::Client(int sockFd)
  : _fd(sockFd)
  , _state(0)
{
}

int Client::getFd() const
{
  return _fd;
}

Client::Buffer Client::getInBuff() const
{
  return _inBuff;
}

Client::Buffer Client::getOutBuff() const
{
  return _outBuff;
}

void Client::addToInBuff(const std::string& str)
{
  _inBuff.insert(_inBuff.end(), str.begin(), str.end());
}

void Client::addToInBuff(const Buffer& buffer)
{
  _inBuff.insert(_inBuff.end(), buffer.begin(), buffer.end());
}

void Client::addToOutBuff(const std::string& str)
{
  _outBuff.insert(_outBuff.end(), str.begin(), str.end());
}

void Client::addToOutBuff(const Buffer& buffer)
{
  _outBuff.insert(_outBuff.end(), buffer.begin(), buffer.end());
}

void Client::removeFromOutBuff(ssize_t bytes)
{
  _outBuff.erase(_outBuff.begin(), _outBuff.begin() + bytes);
}

bool Client::hasDataToSend() const
{
  return !_outBuff.empty();
}
