#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "client/TimeStamp.hpp"
#include "server/Server.hpp"
#include "socket/AutoFd.hpp"
#include "socket/Socket.hpp"
#include "utils/Buffer.hpp"
#include <string>

class Client
{
public:
  Client();
  explicit Client(int fdes);
  explicit Client(int sockFd, const Socket* socket, const Server* server);

  int getFd() const;
  const std::string& getHost() const;
  Buffer getInBuff() const;
  Buffer getOutBuff() const;
  const Socket* getSocket() const;
  const Server* getServer() const;

  void setServer(const Server* server);

  TimeStamp getLastActivity() const;
  void updateLastActivity();
  long getTimeout() const;

  bool hasDataToSend() const;
  bool receive();
  bool sendTo();

private:
  AutoFd _fd;
  // const Socket* _socket; // points to Socket it connected
  const Server* _server; // point to Null in the begining
  std::string _host;
  TimeStamp _lastActivity;
  // int _state;
  Buffer _inBuff;
  Buffer _outBuff;
};

#endif
