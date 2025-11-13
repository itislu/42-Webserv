#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "client/TimeStamp.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
#include "server/Server.hpp"
#include "socket/AutoFd.hpp"
#include "utils/Buffer.hpp"
#include "utils/state/StateHandler.hpp"
#include <cstddef>
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
  const std::string& getHost() const;
  Buffer& getInBuff();
  Buffer& getOutBuff();
  const Server* getServer() const;
  StateHandler<Client>& getStateHandler();
  Request& getRequest();
  Response& getResponse();

  void setServer(const Server* server);

  const TimeStamp& getLastActivity() const;
  std::size_t getTimeout() const;

  bool hasDataToSend() const;
  bool sendTo();
  bool receive();

private:
  void updateLastActivity();

  AutoFd _fd;
  const Server* _server;
  std::string _host;
  TimeStamp _lastActivity;
  Buffer _inBuff;
  Buffer _outBuff;
  StateHandler<Client> _stateHandler;
  Request _request;
  Response _response;
};

#endif
