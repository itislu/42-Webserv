#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "client/TimeStamp.hpp"
#include "http/Request.hpp"
#include "http/Resource.hpp"
#include "http/Response.hpp"
#include "server/Server.hpp"
#include "socket/AutoFd.hpp"
#include "utils/SmartBuffer.hpp"
#include "utils/state/StateHandler.hpp"
#include <string>

#ifndef MAX_CHUNK
#define MAX_CHUNK 1024
#endif

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
  SmartBuffer& getOutBuff();
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

  AutoFd _fd;
  const Server* _server;
  std::string _host;
  TimeStamp _lastActivity;
  SmartBuffer _inBuff;
  SmartBuffer _outBuff;
  StateHandler<Client> _stateHandler;
  Request _request;
  Response _response;
  Resource _resource;
};

#endif
