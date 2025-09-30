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

void Client::addToInBuff(std::string str)
{
  inBuff.insert(inBuff.end(), str.begin(), str.end());
}

void Client::addToInBuff(char* buffer, int bytes)
{
  inBuff.insert(inBuff.end(), buffer, buffer + bytes);
}
