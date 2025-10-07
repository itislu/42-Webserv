
#ifndef SOCKET_HPP
#define SOCKET_HPP

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

private:
  int _port;
  int _fd;
};

#endif