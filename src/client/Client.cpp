#include "Client.hpp"

Client::Client() {}

Client::Client(int fd, struct pollfd* pfd)
{
  _fd = fd;
  _state = 0;
  _pfd = pfd;
}

Client::~Client() {}

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

void Client::addToInBuff(std::string str)
{
  _inBuff.insert(_inBuff.end(), str.begin(), str.end());
}

void Client::addToInBuff(char* buffer, int bytes)
{
  _inBuff.insert(_inBuff.end(), buffer, buffer + bytes);
}

void Client::addToOutBuff(std::string str)
{
  _outBuff.insert(_outBuff.end(), str.begin(), str.end());
  _pfd->events |= POLLOUT;
}

void Client::addToOutBuff(char* buffer, int bytes)
{
  _outBuff.insert(_outBuff.end(), buffer, buffer + bytes);
  _pfd->events |= POLLOUT;
}

void Client::removeFromOutBuff(int bytes)
{
  _outBuff.erase(_outBuff.begin(), _outBuff.begin() + bytes);
  if (!hasDataToSend())
    _pfd->events &= ~POLLOUT;
}

bool Client::hasDataToSend() const
{
  return !_outBuff.empty();
}
