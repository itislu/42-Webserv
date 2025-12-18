#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <http/CgiContext.hpp>
#include <http/Request.hpp>
#include <http/Resource.hpp>
#include <http/Response.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/movable.hpp>
#include <server/Server.hpp>
#include <socket/AutoFd.hpp>
#include <socket/Socket.hpp>
#include <utils/buffer/BufferQueue.hpp>
#include <utils/buffer/SmartBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/StateHandler.hpp>

#include <cstddef>
#include <ostream>
#include <string>

class Client
{
public:
  Client();
  explicit Client(int fdes);
  Client(ft::rvalue<AutoFd>& fdes, const Server* server, const Socket* socket);

  static const std::size_t maxChunk = 1024;

  int getFd() const;
  bool hasServer() const;
  const std::string& getHost() const;
  SmartBuffer& getInBuff();
  BufferQueue& getOutBuffQueue();
  const Server* getServer() const;
  const Socket* getSocket() const;
  StateHandler<Client>& getStateHandler();
  Request& getRequest();
  Response& getResponse();
  Resource& getResource();
  ft::shared_ptr<CgiContext>& getCgiContext();

  void setSocket(const Socket* socket);

  long getTimeout() const;
  bool closeConnection() const;
  bool alive() const;

  void setCloseConnection(bool value);
  void setServer(const Server* server);
  void setAlive(bool value);

  bool hasDataToSend() const;
  bool sendTo();
  bool receive();

  void prepareForNewRequest();

private:
  static Logger& _log;

  AutoFd _fd;
  const Server* _server;
  const Socket* _socket;
  std::string _host;
  SmartBuffer _inBuff;
  BufferQueue _outBuffQueue;
  StateHandler<Client> _stateHandler;
  Request _request;
  Response _response;
  Resource _resource;
  ft::shared_ptr<CgiContext> _cgiContext;

  bool _closeConnection;
  bool _alive;
};

std::ostream& operator<<(std::ostream& out, const Client& client);

#endif
