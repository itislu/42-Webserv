#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "client/TimeStamp.hpp"
#include "server/Server.hpp"
#include "socket/AutoFd.hpp"
#include "utils/Buffer.hpp"
#include <string>

#ifndef MAX_CHUNK
#define MAX_CHUNK 1024
#endif

class Client
{
public:
  Client();
  explicit Client(int fdes);
  explicit Client(int fdes, const Server* server);

  int getFd() const;
  const std::string& getHost() const;
  Buffer getInBuff() const;
  Buffer getOutBuff() const;
  const Server* getServer() const;

  void setServer(const Server* server);

  const TimeStamp& getLastActivity();
  void updateLastActivity();
  long getTimeout() const;

  bool hasDataToSend() const;
  bool receive();
  bool sendTo();

private:
  // int _state;
  AutoFd _fd;
  const Server* _server;
  std::string _host;
  TimeStamp _lastActivity;
  Buffer _inBuff;
  Buffer _outBuff;
};

#endif
