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

  explicit ServerHandler(const Config& config);

  void run();
  void init();
  void createServers(const Config& config);
  void initListeners();
  void addToPfd(int sockFd);
  bool isListener(int sockFd);

  std::vector<const Socket*> createListeners(const std::vector<int>& ports);
  void mapServerToListeners(const std::vector<int>& ports, Server* server);
  const Socket* getListener(int port);

  void debugPrintMaps() const;

  void addListenersToPfd(Server* server);

  void mapListeners();
  void addServerToListener();

private:
  std::vector<Server*> _servers;
  std::vector<pollfd> _pfds;
  std::map<const Socket*, std::vector<Server*> > _socketToServers;
  std::map<int, const Socket*> _portToSocket;
};

#endif
