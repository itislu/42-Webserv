#include "Socket.hpp"
#include "socket/AutoFd.hpp"
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
  initSocket();
}

int Socket::getRawFd() const
{
  return _fd.get();
}

const AutoFd& Socket::getFd() const
{
  return _fd;
}

int Socket::getPort() const
{
  return _port;
}

void Socket::throwSocketException(const std::string& msg)
{
  if (_fd.get() >= 0) {
    close(_fd.get());
  }
  throw std::runtime_error(msg);
}

struct sockaddr_in Socket::getIpv4SockAddr() const
{
  struct sockaddr_in sockAddr = {};
  sockAddr.sin_family = AF_INET;
  sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  sockAddr.sin_port = htons(_port);

  return sockAddr;
}

void Socket::setFlags()
{
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg): POSIX C API.
  if (fcntl(_fd.get(), F_SETFL, O_NONBLOCK) < 0) {
    throwSocketException("failed to set server socket to non-blocking");
  }
}

void Socket::initSocket()
{
  _fd.set(socket(AF_INET, SOCK_STREAM, 0));
  if (_fd.get() < 0) {
    throwSocketException("server socket creation failed");
  }

  int opt = 1;
  if (setsockopt(_fd.get(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    throwSocketException("setting options for server socket failed");
  }

  struct sockaddr_in sockAddr = getIpv4SockAddr();

  // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
  if (bind(_fd.get(),
           reinterpret_cast<const struct sockaddr*>(&sockAddr),
           sizeof(sockAddr)) < 0)
  // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
  {
    throwSocketException("failed to bind server socket");
  }

  if (listen(_fd.get(), SOMAXCONN) < 0) {
    throwSocketException("failed to set server socket to listen");
  }
  setFlags();
}
