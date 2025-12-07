#include "Client.hpp"

#include <client/TimeStamp.hpp>
#include <config/Config.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/Resource.hpp>
#include <http/Response.hpp>
#include <http/StatusCode.hpp>
#include <http/Uri.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <server/Server.hpp>
#include <socket/AutoFd.hpp>
#include <utils/buffer/BufferQueue.hpp>
#include <utils/buffer/IBuffer.hpp>
#include <utils/buffer/IInBuffer.hpp>
#include <utils/buffer/MemoryBuffer.hpp>
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
  updateLastActivity();
  return true;
}

bool Client::sendTo()
{
  const ssize_t bytes = _outBuffQueue.send(getFd(), maxChunk);
  if (bytes > 0) {
    _log.info() << "sent " << bytes << " bytes\n";
  } else if (bytes == 0) {
    _log.warning() << "no data sent to client fd=" << getFd() << "\n";
  } else {
    _log.error() << "send error for client fd=" << getFd() << ": "
                 << std::strerror(errno) << "\n";
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

  Response& response = getResponse();
  response.setStatusCode(StatusCode::Ok);
  response.setVersion("");
  response.setBody(ft::shared_ptr<MemoryBuffer>());
  response.getHeaders() = Headers();

  Request& request = getRequest();
  request.setMethod(Request::UNDEFINED);
  request.setUri(Uri());
  request.setVersion("");
  request.getHeaders() = Headers();

  Resource& resource = getResource();
  resource.setLocation(FT_NULLPTR);
  resource.setPath("");
  resource.setServer(FT_NULLPTR);
}
