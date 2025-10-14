#ifndef SERVERHANDLER_HPP
#define SERVERHANDLER_HPP

#include "Server.hpp"
#include "config/Config.hpp"
#include "config/ServerConfig.hpp"
#include "socket/Socket.hpp"
#include <map>
#include <sys/poll.h>
#include <vector>

class ServerHandler
{
public:
  typedef std::map<int, const Socket*>::const_iterator portToSocketIter;
  typedef std::map<const Socket*, std::vector<Server*> >::const_iterator
    socketToServersIter;

  // CONSTRUCTORS
  explicit ServerHandler(const Config& config);
  ~ServerHandler();

  // RUN - Event Loop
  void run();
  void checkActivity();
  bool isListener(int sockFd);
  void acceptClient(int sockFd);

  // INIT - Server Setup
  void createServers(const Config& config);
  void initListeners();
  void addToPfd(int sockFd);
  std::vector<const Socket*> createListeners(const std::vector<int>& ports);
  void mapServerToListeners(const std::vector<int>& ports, Server* server);
  const Socket* getListener(int port);
  void mapListeners();
  void addServerToListener();

  // DEBUG
  void debugPrintMaps() const;

private:
  ServerHandler(const ServerHandler& other);
  ServerHandler& operator=(const ServerHandler& other);

  std::vector<Server*> _servers;
  std::vector<pollfd> _pfds; // listeners + clients
  std::map<const Socket*, std::vector<Server*> > _socketToServers;
  std::map<int, const Socket*> _portToSocket;
};

#endif
