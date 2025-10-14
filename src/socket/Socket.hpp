
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "AutoFd.hpp"
#include <string>

class Socket
{
public:
  explicit Socket(int port);

  int getFd() const;
  int getPort() const;

  void initSocket();
  void throwSocketException(const std::string& msg);
  void makeNonBlocking(int sockFd);

  struct sockaddr_in getIpv4SockAddr() const;
  static void setFlags(int sockFd);

private:
  int _port;
  AutoFd _fd;
};

#endif
