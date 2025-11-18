#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "Server.hpp"
#include "client/ClientManager.hpp"
#include "config/ServerConfig.hpp"
#include "event/EventManager.hpp"
#include "libftpp/memory.hpp"
#include "socket/Socket.hpp"
#include "socket/SocketManager.hpp"
#include <cstddef>
#include <map>
#include <vector>

class ServerManager
{
public:
  explicit ServerManager(const config::Config& config);
  ~ServerManager() {}

  const Server* getServerFromSocket(const Socket* socket) const;
  const std::vector<ft::shared_ptr<const Server> >& getServers() const;
  const Server* getInitServer(int fdes) const;

  void run();

  std::size_t serverCount() const;

private:
  typedef std::map<const Socket*, std::vector<const Server*> >::iterator
    SockToServIter;
  typedef std::map<const Socket*, std::vector<const Server*> >::const_iterator
    const_SockToServIter;

  void addServer(const config::ServerConfig& config,
                 const std::vector<const Socket*>& listeners);
  void createServers(const std::vector<config::ServerConfig>& configs);
  std::vector<const Socket*> createListeners(const std::vector<int>& ports);
  void mapServerToSocket(const Server& server,
                         const std::vector<const Socket*>& listeners);

  ServerManager(const ServerManager& other);
  ServerManager& operator=(const ServerManager& other);

  const config::Config* _config;
  SocketManager _socketManager;
  ClientManager _clientManager;
  EventManager _eventManager;
  std::vector<ft::shared_ptr<const Server> > _servers;
  std::map<const Socket*, std::vector<const Server*> > _socketToServers;
};

#endif
