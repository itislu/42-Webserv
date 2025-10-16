#include "Client.hpp"
#include "server/Server.hpp"
#include "socket/AutoFd.hpp"
#include "socket/Socket.hpp"
#include "utils/Buffer.hpp"
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

bool Client::hasDataToSend() const
{
  return !_outBuff.isEmpty();
}
