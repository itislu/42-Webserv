#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <client/TimeStamp.hpp>
#include <http/Request.hpp>
#include <http/Resource.hpp>
#include <http/Response.hpp>
#include <ostream>
#include <server/Server.hpp>
#include <socket/AutoFd.hpp>
#include <utils/buffer/BufferQueue.hpp>
#include <utils/buffer/SmartBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/StateHandler.hpp>

#include <cstddef>
#include <string>

class Client
{
public:
  Client();
  explicit Client(int fdes);
  Client(int fdes, const Server* server);

  int getFd() const;
  bool hasServer() const;
  const std::string& getHost() const;
  SmartBuffer& getInBuff();
  BufferQueue& getOutBuffQueue();
  const Server* getServer() const;
  StateHandler<Client>& getStateHandler();
  Request& getRequest();
  Response& getResponse();
  Resource& getResource();

  void setServer(const Server* server);

  const TimeStamp& getLastActivity() const;
  long getTimeout() const;

  bool hasDataToSend() const;
  bool sendTo();
  bool receive();

private:
  void updateLastActivity();

  static Logger& _log;
  static const std::size_t _maxChunk = 1024;

  AutoFd _fd;
  const Server* _server;
  std::string _host;
  TimeStamp _lastActivity;
  SmartBuffer _inBuff;
  BufferQueue _outBuffQueue;
  StateHandler<Client> _stateHandler;
  Request _request;
  Response _response;
  Resource _resource;
};

std::ostream& operator<<(std::ostream& out, const Client& client);

#endif
