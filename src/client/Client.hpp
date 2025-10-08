#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "utils/Buffer.hpp"

class Client
{
public:
  Client();
  explicit Client(int sockFd);

  int getFd() const;
  Buffer getInBuff() const;
  Buffer getOutBuff() const;

  bool hasDataToSend() const;

private:
  int _fd;
  // int _state;
  Buffer _inBuff;
  Buffer _outBuff;
};

#endif
