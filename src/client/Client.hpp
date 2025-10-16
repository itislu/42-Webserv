#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "server/Server.hpp"
#include "socket/AutoFd.hpp"
#include "socket/Socket.hpp"
#include "utils/Buffer.hpp"
#include <string>

class Client
{
public:
  Client();
  explicit Client(int sockFd);

  int getFd() const;
  const std::string& getHost() const;
  Buffer getInBuff() const;
  Buffer getOutBuff() const;

  bool hasDataToSend() const;

private:
  AutoFd _fd;
  // Socket* _socket; // set pointer to the socket it belongs to
  // Server* _server; // point to Null in the begining
  std::string _host;
  // int _state;
  Buffer _inBuff;
  Buffer _outBuff;
};

#endif
