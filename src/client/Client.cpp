#include "Client.hpp"
#include <cstddef>
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

Client::Client(const Client& other)
  : _fd()
  , _state()
{
  *this = other;
}

Client& Client::operator=(const Client& other)
{
  if (this != &other) {
    _fd = other._fd;
    _state = other._state;
    _inBuff = other._inBuff;
    _outBuff = other._outBuff;
  }
  return *this;
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

void Client::addToInBuff(std::vector<char>& buffer)
{
  _inBuff.insert(_inBuff.end(), buffer.begin(), buffer.end());
}

void Client::addToOutBuff(std::string str)
{
  _outBuff.insert(_outBuff.end(), str.begin(), str.end());
}

void Client::addToOutBuff(std::vector<char>& buffer)
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
