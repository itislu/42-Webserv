#include "Client.hpp"

Client::Client(int fd)
{
  _fd = fd;
  _state = 0;
}

Client::~Client()
{

}

int Client::getFd() const
{
  return _fd;
}

int Client::getBytes() const
{
  return _bytes;
}

void Client::setBytes(int bytes)
{
  _bytes = bytes;
}

void Client::addToInBuff(char* buffer, int bytes)
{
  inBuff.insert(inBuff.end(), buffer, buffer + bytes);
}
