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
  typedef std::vector<ft::shared_ptr<const Server> > Servers;

  explicit ServerManager(const Config& config);
  ~ServerManager() {}

  const Server* getServerFromSocket(const Socket* socket) const;
  const Servers& getServers() const;
  const Server* getInitServer(int fdes) const;

  void run();

  std::size_t serverCount() const;

private:
  typedef std::map<const Socket*, std::vector<const Server*> > SockToServ;
  typedef SockToServ::iterator SockToServIter;
  typedef SockToServ::const_iterator const_SockToServIter;

  void addServer(const ServerConfig& config,
                 const Server::Listeners& listeners);
  void createServers(const Config::ServerConfigs& configs);
  Server::Listeners createListeners(const std::vector<int>& ports);
  void mapServerToSocket(const Server& server,
                         const Server::Listeners& listeners);

  ServerManager(const ServerManager& other);
  ServerManager& operator=(const ServerManager& other);

  const Config* _config;
  SocketManager _socketManager;
  ClientManager _clientManager;
  EventManager _eventManager;
  Servers _servers;
  SockToServ _socketToServers;
};

#endif
