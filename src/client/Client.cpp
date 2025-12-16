#include "Client.hpp"

#include <config/Config.hpp>
#include <http/CgiContext.hpp>
#include <http/Request.hpp>
#include <http/Resource.hpp>
#include <http/Response.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <server/Server.hpp>
#include <socket/AutoFd.hpp>
#include <socket/Socket.hpp>
#include <utils/buffer/BufferQueue.hpp>
#include <utils/buffer/IBuffer.hpp>
#include <utils/buffer/IInBuffer.hpp>
#include <utils/buffer/SmartBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/StateHandler.hpp>

#include <cerrno>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

/* **************************************************************************
 */
// Init

Logger& Client::_log = Logger::getInstance(LOG_SERVER);

const std::size_t Client::maxChunk;

/* ************************************************************************** */

Client::Client()
  : _fd(-1)
  , _server()
  , _socket()
  , _stateHandler(this)
  , _closeConnection(false)
  , _alive(true)
{
  _stateHandler.setState<ReadRequestLine>();
}

Client::Client(int fdes)
  : _fd(fdes)
  , _server()
  , _socket()
  , _stateHandler(this)
  , _closeConnection(false)
  , _alive(true)
{
  _stateHandler.setState<ReadRequestLine>();
}

Client::Client(int fdes, const Server* server, const Socket* socket)
  : _fd(fdes)
  , _server(server)
  , _socket(socket)
  , _stateHandler(this)
  , _closeConnection(false)
  , _alive(true)
{
  _stateHandler.setState<ReadRequestLine>();
}

int Client::getFd() const
{
  return _fd.get();
}

bool Client::hasServer() const
{
  return (_server != FT_NULLPTR);
}

const std::string& Client::getHost() const
{
  return _host;
}

SmartBuffer& Client::getInBuff()
{
  return _inBuff;
}

BufferQueue& Client::getOutBuffQueue()
{
  return _outBuffQueue;
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

Resource& Client::getResource()
{
  return _resource;
}

ft::shared_ptr<CgiContext>& Client::getCgiContext()
{
  return _cgiContext;
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

const Socket* Client::getSocket() const
{
  return _socket;
}

void Client::setServer(const Server* server)
{
  _server = server;
}

void Client::setSocket(const Socket* socket)
{
  _socket = socket;
}

bool Client::receive()
{
  static IInBuffer::RawBytes buffer(maxChunk);
  const ssize_t bytes = recv(getFd(), buffer.data(), buffer.size(), 0);
  if (bytes > 0) {
    /* TODO: remove this! */
    std::cout << "Client " << getFd() << ": ";
    // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
    std::cout.write(reinterpret_cast<const char*>(buffer.data()),
                    static_cast<std::streamsize>(bytes));
    // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
    _inBuff.append(buffer, bytes);
  } else if (bytes == 0) {
    std::cout << "[CLIENT] wants to disconnect\n";
    return false;
  } else // bytes < 0
  {
    std::cerr << ("[SERVER] recv failed, removing client\n");
    return false;
  }
  return true;
}

bool Client::sendTo()
{
  const ssize_t bytes = _outBuffQueue.send(getFd(), maxChunk);
  if (bytes > 0) {
    _log.info() << *this << "sent " << bytes << " bytes\n";
  } else if (bytes == 0) {
    _log.warning() << *this << "no data sent to client fd=" << getFd() << "\n";
  } else {
    _log.error() << *this << "send error for client fd=" << getFd() << ": "
                 << std::strerror(errno) << "\n";
    return false;
  }
  return true;
}

bool Client::hasDataToSend() const
{
  return !_outBuffQueue.isEmpty();
}

std::ostream& operator<<(std::ostream& out, const Client& client)
{
  out << "Client(" << client.getFd() << ")";
  return out;
}

void Client::prepareForNewRequest()
{
  getStateHandler().setState<ReadRequestLine>();

  _response = Response();
  _request = Request();
  _resource = Resource();

  _cgiContext.reset();
}

void Client::setCloseConnection(bool value)
{
  _closeConnection = value;
}

bool Client::closeConnection() const
{
  return _closeConnection;
}

void Client::setAlive(bool value)
{
  _alive = value;
}

bool Client::alive() const
{
  return _alive;
}
