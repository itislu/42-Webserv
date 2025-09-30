#include "client/Client.hpp"
#include "http/Request.hpp"
#include <cstddef>
#include <string>
#include <unistd.h>

Client::Client() {}

Client::Client(int fd)
{
  _fd = fd;
}

Client::~Client()
{
  // std::cout << "[Client] Destructor called\n";
  // close(_fd);
}

int Client::getFd() const
{
  return _fd;
}

Client::Buffer& Client::getBuffer()
{
  return _inBuff;
}

void Client::inBuffAddStr(std::string& str)
{
  for (std::size_t i = 0; i < str.size(); i++) {
    _inBuff.push_back(str[i]);
  }
}

Request& Client::getRequest()
{
  return _request;
}
