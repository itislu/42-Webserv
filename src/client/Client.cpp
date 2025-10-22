#include "Client.hpp"
#include "http/Request.hpp"
#include "utils/Buffer.hpp"
#include <string>
#include <sys/types.h>
#include <vector>

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

Buffer& Client::getInBuff()
{
  return _inBuff;
}

Buffer& Client::getOutBuff()
{
  return _outBuff;
}

Request& Client::getRequest()
{
  return _request;
}

// void Client::addToInBuff(std::string str)
// {
//   _inBuff.insert(_inBuff.end(), str.begin(), str.end());
// }

// void Client::addToInBuff(std::vector<char>& buffer)
// {
//   _inBuff.insert(_inBuff.end(), buffer.begin(), buffer.end());
// }

// void Client::addToOutBuff(std::string str)
// {
//   _outBuff.insert(_outBuff.end(), str.begin(), str.end());
// }

// void Client::addToOutBuff(std::vector<char>& buffer)
// {
//   _outBuff.insert(_outBuff.end(), buffer.begin(), buffer.end());
// }

// void Client::removeFromOutBuff(ssize_t bytes)
// {
//   _outBuff.erase(_outBuff.begin(), _outBuff.begin() + bytes);
// }

// bool Client::hasDataToSend() const
// {
//   return !_outBuff.empty();
// }
