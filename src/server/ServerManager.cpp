#include "ServerManager.hpp"
#include "config/Config.hpp"
#include "config/ServerConfig.hpp"
#include "libftpp/memory.hpp"
#include "libftpp/utility.hpp"
#include "server/Server.hpp"
#include "socket/Socket.hpp"
#include "socket/SocketManager.hpp"
#include <cerrno>
#include <csignal>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

static volatile std::sig_atomic_t g_running = 0;

static void error(const std::string& msg)
{
  std::cerr << "Error: " << msg << " (" << std::strerror(errno) << ")\n";
}

extern "C" void sigIntHandler(int /*sigNum*/)
{
  g_running = 0;
}

ServerManager::ServerManager(const config::Config& config)
  : _config(&config)
  , _socketManager(config)
  , _eventManager(_clientManager, _socketManager, *this)
{
  if (std::signal(SIGINT, sigIntHandler) == SIG_ERR) {
    throw std::runtime_error("Failed to set SIGINT handler");
  }
  createServers(_config->getServers());
}

void ServerManager::createServers(const std::vector<config::ServerConfig>& configs)
{
  _servers.reserve(configs.size());
  for (config::Config::const_ServConfIter it = configs.begin(); it != configs.end();
       ++it) {
    const std::vector<const Socket*> listeners =
      createListeners(it->getPorts());
    addServer(*it, listeners);
  }
}

std::vector<const Socket*> ServerManager::createListeners(
  const std::vector<int>& ports)
{
  std::vector<const Socket*> listeners;
  listeners.reserve(ports.size());
  for (std::vector<int>::const_iterator it = ports.begin(); it != ports.end();
       ++it) {
    const int port = *it;
    const Socket& sock = _socketManager.getListener(port);
    listeners.push_back(&sock);
  }
  return listeners;
}

void ServerManager::addServer(const config::ServerConfig& config,
                              const std::vector<const Socket*>& listeners)
{
  const ft::shared_ptr<const Server> server =
    ft::make_shared<const Server>(config, listeners);
  _servers.push_back(server);
  mapServerToSocket(*server, listeners);
}

void ServerManager::mapServerToSocket(
  const Server& server,
  const std::vector<const Socket*>& listeners)
{
  for (std::vector<const Socket*>::const_iterator it = listeners.begin();
       it != listeners.end();
       ++it) {
    _socketToServers[*it].push_back(&server);
  }
}

const Server* ServerManager::getServerFromSocket(const Socket* socket) const
{
  if (socket == FT_NULLPTR) {
    return FT_NULLPTR;
  }
  const const_SockToServIter iter = _socketToServers.find(socket);
  if (iter == _socketToServers.end()) {
    return FT_NULLPTR;
  }
  const std::vector<const Server*>& servers = iter->second;
  if (servers.size() != 1) {
    return FT_NULLPTR;
  }
  return servers[0];
}

/*
  It should never happen that no server is found for this socket.
  However, if the socket is associated with multiple servers, we can only
  return a specific server once we parse the Host header.
  Until then, return nullptr to indicate client is not associated with a
  specific server yet.
*/
const Server* ServerManager::getInitServer(int fdes) const
{
  const Socket& socket = _socketManager.getSocket(fdes);
  const const_SockToServIter iter = _socketToServers.find(&socket);
  if (iter != _socketToServers.end() && iter->second.size() == 1) {
    return iter->second[0];
  }
  return FT_NULLPTR;
}

std::size_t ServerManager::serverCount() const
{
  return _servers.size();
}

const std::vector<ft::shared_ptr<const Server> >& ServerManager::getServers()
  const
{
  return _servers;
}

void ServerManager::run()
{
  g_running = 1;
  while (g_running == 1) {
    const int res = _eventManager.check();
    if (res == 0) {
      std::cout << "poll: timeout\n";
    }
    if (res < 0) {
      if (errno != EINTR) {
        error("poll: failed");
      }
      break;
    }
    _eventManager.checkTimeouts();
  }
  std::cout << "Shutting down servers...\n";
}
