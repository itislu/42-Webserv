#include "Client.hpp"
#include "config/ServerConfig.hpp"
#include "server/Server.hpp"
#include "socket/AutoFd.hpp"
#include "socket/Socket.hpp"
#include "utils/Buffer.hpp"
#include "utils/TimeStamp.hpp"
#include <asm-generic/errno.h>
#include <iostream>
#include <string>

Client::Client()
  : _fd(-1)
  , _socket()
  , _server()
//, _state(0)
{
}

Client::Client(int sockFd, const Socket* socket, const Server* server)
  : _fd(sockFd)
  , _socket(socket)
  , _server(server)
//, _state(0)
{
  std::cout << "[CLIENT] connected " << _lastActivity.getTime() << "\n";
}

int Client::getFd() const
{
  return _fd.get();
}

const std::string& Client::getHost() const
{
  return _host;
}

Buffer Client::getInBuff() const
{
  return _inBuff;
}

Buffer Client::getOutBuff() const
{
  return _outBuff;
}

const Socket* Client::getSocket() const
{
  return _socket;
}

const Server* Client::getServer() const
{
  return _server;
}

void Client::setServer(const Server* server)
{
  _server = server;
}

TimeStamp Client::getLastActivity() const
{
  return _lastActivity;
}

void Client::updateLastActivity()
{
  _lastActivity.setTime(TimeStamp::now());
}

bool Client::hasDataToSend() const
{
  return !_outBuff.isEmpty();
}
