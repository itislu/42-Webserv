#include "Client.hpp"

Client::Client(int fd)
{
  _fd = fd;
  _state = 0;
}

Client::~Client() {}

int Client::getFd() const
{
  return _fd;
}

Client::Buffer Client::getInBuff() const
{
  return inBuff;
}

Client::Buffer Client::getOutBuff()
{
  return outBuff;
}

void Client::addToInBuff(std::string str)
{
  inBuff.insert(inBuff.end(), str.begin(), str.end());
}

void Client::addToInBuff(char* buffer, int bytes)
{
  inBuff.insert(inBuff.end(), buffer, buffer + bytes);
}

void Client::addToOutBuff(std::string str)
{
  outBuff.insert(outBuff.end(), str.begin(), str.end());
}

void Client::addToOutBuff(char* buffer, int bytes)
{
  outBuff.insert(outBuff.end(), buffer, buffer + bytes);
}
