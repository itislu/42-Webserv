#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <vector>
#include <map>
#include "Server.hpp"
#include "socket/SocketManager.hpp"
#include "client/ClientManager.hpp"
#include "event/EventManager.hpp"

class ServerManager {

  typedef std::map<const Socket*, std::vector<const Server*> >::iterator sockToServIter;
  typedef std::map<const Socket*, std::vector<const Server*> >::const_iterator c_sockToServIter;

public:
  explicit ServerManager(const Config& config);
  ~ServerManager();

  void addServer(const ServerConfig& cfg, const std::vector<const Socket*>& listeners);
  void createServers(const std::vector<ServerConfig>& configs);
  std::vector<const Socket*> createListeners(const std::vector<int>& ports);
  const Socket* getListener(int port);

  const Server* getServerFromSocket(const Socket* socket) const;
  const std::vector<Server*>& getServers();

  void mapServerToSocket(const Server* server, const std::vector<const Socket*>& listeners);

  void run();

  size_t serverCount() const;

private:
  ServerManager(const ServerManager& other);
  ServerManager& operator=(const ServerManager& other);

  const Config& _config;
  SocketManager _socketManager;
  ClientManager _clientManager;
  EventManager _eventManager;
  std::vector<Server*> _servers;
  std::map<const Socket*, std::vector<const Server*> > _socketToServers;
};

#endif
