#include "Client.hpp"
#include "client/TimeStamp.hpp"
#include "config/Config.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
#include "http/states/readRequestLine/ReadRequestLine.hpp"
#include "libftpp/format.hpp"
#include "libftpp/utility.hpp"
#include "server/Server.hpp"
#include "socket/AutoFd.hpp"
#include "utils/Buffer.hpp"
#include "utils/state/StateHandler.hpp"
#include <algorithm>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

Client::Client()
  : _fd(-1)
  , _server()
  , _stateHandler(this)
{
  _stateHandler.setState<ReadRequestLine>();
}

Client::Client(int fdes)
  : _fd(fdes)
  , _server()
  , _stateHandler(this)
{
  _stateHandler.setState<ReadRequestLine>();
}

Client::Client(int fdes, const Server* server)
  : _fd(fdes)
  , _server(server)
  , _stateHandler(this)
{
  _stateHandler.setState<ReadRequestLine>();
}

int Client::getFd() const
{
  return _fd.get();
}

const std::string& Client::getHost() const
{
  return _host;
}

Buffer& Client::getInBuff()
{
  return _inBuff;
}

Buffer& Client::getOutBuff()
{
  return _outBuff;
}

StateHandler<Client>& Client::getStateHandler()
{
  return _stateHandler;
}

Request& Client::getRequest()
{
  return _request;
}

Response& Client::getResponse()
{
  return _response;
}

long Client::getTimeout() const
{
  if (_server != FT_NULLPTR) {
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
  static std::vector<unsigned char> buffer(MAX_CHUNK);
  const ssize_t bytes = recv(getFd(), buffer.data(), buffer.size(), 0);
  if (bytes > 0) {
    std::cout << "Client " << getFd() << ": ";
    // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
    std::cout.write(reinterpret_cast<const char*>(buffer.data()),
                    static_cast<std::streamsize>(bytes));
    // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
    _inBuff.add(buffer, bytes);
    // TODO: STATEMACHINE/PARSING
    _stateHandler.setStateHasChanged(true);
    while (!_stateHandler.isDone() && _stateHandler.stateHasChanged()) {
      _stateHandler.setStateHasChanged(false);
      _stateHandler.getState()->run();
    }
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

const TimeStamp& Client::getLastActivity() const
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
