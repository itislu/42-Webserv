#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "Server.hpp"
#include "client/ClientManager.hpp"
#include "config/ServerConfig.hpp"
#include "event/EventManager.hpp"
#include "socket/Socket.hpp"
#include "socket/SocketManager.hpp"
#include <cstddef>
#include <map>
#include <vector>

class ServerManager
{
  typedef std::map<const Socket*, std::vector<const Server*> >::iterator
    sockToServIter;
  typedef std::map<const Socket*, std::vector<const Server*> >::const_iterator
    const_sockToServIter;

public:
  explicit ServerManager(const Config& config);
  ~ServerManager();

  const Server* getServerFromSocket(const Socket* socket) const;
  const std::vector<const Server*>& getServers() const;
  const Server* getInitServer(int fdes) const;

  void run();

  std::size_t serverCount() const;

private:
  void addServer(const ServerConfig& config,
                 const std::vector<const Socket*>& listeners);
  void createServers(const std::vector<ServerConfig>& configs);
  std::vector<const Socket*> createListeners(const std::vector<int>& ports);
  void mapServerToSocket(const Server* server,
                         const std::vector<const Socket*>& listeners);

  ServerManager(const ServerManager& other);
  ServerManager& operator=(const ServerManager& other);

  const Config* _config;
  SocketManager _socketManager;
  ClientManager _clientManager;
  EventManager _eventManager;
  std::vector<const Server*> _servers;
  std::map<const Socket*, std::vector<const Server*> > _socketToServers;
};

#endif
