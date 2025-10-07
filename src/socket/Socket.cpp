#include "Socket.hpp"
#include "socket/Socket.hpp"
#include <cstring>      //std::memset()
#include <fcntl.h>      //fcntl()
#include <netinet/in.h> //struct sockaddr
#include <stdexcept>
#include <string>
#include <sys/socket.h> //socket(), setsockopt(),
#include <unistd.h>

Socket::Socket(int port)
  : _port(port)
  , _fd(-1)
{
}

int Socket::getFd() const
{
  return _fd;
}

int Socket::getPort() const
{
  return _port;
}

void Socket::throwSocketException(const std::string& msg)
{
  close(_fd);
  _fd = -1;
  throw std::runtime_error(msg);
}

struct sockaddr_in Socket::getIpv4SockAddr() const
{
  struct sockaddr_in sockAddr = {};
  std::memset(&sockAddr, 0, sizeof(sockAddr));
  sockAddr.sin_family = AF_INET;
  sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  sockAddr.sin_port = htons(_port);

  return sockAddr;
}

void Socket::setFlags()
{
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg): POSIX C API.
  if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0) {
    throwSocketException("failed to set server socket to non-blocking");
  }
}

void Socket::initSocket()
{
  _fd = socket(AF_INET, SOCK_STREAM, 0);
  if (_fd < 0) {
    throwSocketException("server socket creation failed");
  }

  int opt = 1;
  if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    throwSocketException("setting options for server socket failed");
  }

  struct sockaddr_in sockAddr = getIpv4SockAddr();

  // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
  if (bind(_fd,
           reinterpret_cast<const struct sockaddr*>(&sockAddr),
           sizeof(sockAddr)) < 0)
  // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
  {
    throwSocketException("failed to bind server socket");
  }

  if (listen(_fd, SOMAXCONN) < 0) {
    throwSocketException("failed to set server socket to listen");
  }
  setFlags();
}
