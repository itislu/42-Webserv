

#ifndef SOCKET_HPP
#define SOCKET_HPP

class Socket
{
public:
  explicit Socket(int port);

  int getFd() const;
  int getPort() const;

private:
  int _port;
  int _fd;
};

#endif