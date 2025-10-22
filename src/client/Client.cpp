#include "Client.hpp"
#include "client/TimeStamp.hpp"
#include "config/Config.hpp"
#include "server/Server.hpp"
#include "socket/AutoFd.hpp"
#include "utils/Buffer.hpp"
#include <algorithm>
#include <cerrno>
#include <climits>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

Client::Client()
  : _fd(-1)
  //, _state(0)
  , _server()
{
}

Client::Client(int fdes)
  : _fd(fdes)
  , _server()
{
}

Client::Client(int fdes, const Server* server)
  : _fd(fdes)
  , _server(server)
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

long Client::getTimeout() const
{
  if (_server != 0) {
    return _server->getTimeout();
  }
  return Config::getDefaultTimeout();
}

const Server* Client::getServer() const
{
  return _server;
}

void Client::setServer(const Server* server)
{
  _server = server;
}

bool Client::receive()
{
  std::vector<unsigned char> buffer(MAX_CHUNK);
  const ssize_t bytes = recv(getFd(), buffer.data(), buffer.size(), 0);
  if (bytes > 0) {
    std::cout << "Client " << getFd() << ": ";
    // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
    std::cout.write(reinterpret_cast<const char*>(buffer.data()),
                    static_cast<std::streamsize>(bytes));
    // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
    _inBuff.add(buffer);

    // TODO: STATEMACHINE/PARSING
  } else if (bytes == 0) {
    std::cout << "[CLIENT] wants to disconnect\n";
    return false;
  } else // bytes < 0
  {
    std::cerr << ("[SERVER] recv failed, removing client\n");
    return false;
  }
  updateLastActivity();
  return true;
}

bool Client::sendTo()
{
  const std::size_t toSend =
    std::min(_outBuff.getSize(), static_cast<std::size_t>(MAX_CHUNK));
  const ssize_t bytes = send(getFd(), _outBuff.data(), toSend, 0);

  if (bytes > 0) {
    _outBuff.remove(bytes);
  } else if (bytes == 0) {
    std::cout << "[SERVER] no data sent to client fd=" << getFd() << "\n";
  } else {
    std::cerr << "[SERVER] send error for client fd=" << getFd() << ": "
              << strerror(errno) << "\n";
    return false;
  }
  updateLastActivity();
  return true;
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
