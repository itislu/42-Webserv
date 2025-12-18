#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "Server.hpp"

#include <config/ServerConfig.hpp>
#include <event/EventManager.hpp>
#include <libftpp/memory.hpp>
#include <socket/Socket.hpp>
#include <utils/logger/Logger.hpp>

#include <cstddef>
#include <map>
#include <string>
#include <vector>

class ServerManager
{
public:
  static ServerManager& getInstance();

  ~ServerManager() {}

  const std::vector<ft::shared_ptr<const Server> >& getServers() const;
  const Server* getInitServer(const Socket& socket) const;
  const Server* getServerByHost(const Socket* socket,
                                const std::string& host) const;

  static void run();

  std::size_t serverCount() const;

private:
  typedef std::map<const Socket*, std::vector<const Server*> >::iterator
    SockToServIter;
  typedef std::map<const Socket*, std::vector<const Server*> >::const_iterator
    const_SockToServIter;

  void addServer(const ServerConfig& config,
                 const std::vector<const Socket*>& listeners);
  void createServers(const std::vector<ServerConfig>& configs);
  void mapServerToSocket(const Server& server,
                         const std::vector<const Socket*>& listeners);

  static std::vector<const Socket*> createListeners(
    const std::vector<int>& ports);

  explicit ServerManager(const Config& config);
  ServerManager(const ServerManager& other);
  ServerManager& operator=(const ServerManager& other);

  static Logger& _log;
  std::vector<ft::shared_ptr<const Server> > _servers;
  std::map<const Socket*, std::vector<const Server*> > _socketToServers;
};

#endif
