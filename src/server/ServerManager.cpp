#include "ServerManager.hpp"

#include <config/Config.hpp>
#include <config/ServerConfig.hpp>
#include <event/EventManager.hpp>
#include <libftpp/algorithm.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/string.hpp>
#include <libftpp/utility.hpp>
#include <server/Server.hpp>
#include <socket/Socket.hpp>
#include <socket/SocketManager.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/process/ChildProcessManager.hpp>

#include <cassert>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <stdexcept>
#include <string>
#include <sys/signal.h>
#include <vector>

/* ***************************************************************************/
// INIT

static volatile std::sig_atomic_t g_running = 0;

Logger& ServerManager::_log = Logger::getInstance(LOG_SERVER);

/* ************************************************************************** */

extern "C" void sigIntHandler(int /*sigNum*/)
{
  g_running = 0;
}

ServerManager& ServerManager::getInstance()
{
  static ServerManager serverManager(Config::getConfig());

  return serverManager;
}

ServerManager::ServerManager(const Config& config)
{
  if (std::signal(SIGINT, sigIntHandler) == SIG_ERR) {
    throw std::runtime_error("Failed to set SIGINT handler");
  }
  createServers(config.getServers());
}

void ServerManager::createServers(const std::vector<ServerConfig>& configs)
{
  _servers.reserve(configs.size());
  for (Config::const_ServConfIter it = configs.begin(); it != configs.end();
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
  const SocketManager& socketManager = SocketManager::getInstance();
  for (std::vector<int>::const_iterator it = ports.begin(); it != ports.end();
       ++it) {
    const int port = *it;
    const Socket& sock = socketManager.getListener(port);
    listeners.push_back(&sock);
  }
  return listeners;
}

void ServerManager::addServer(const ServerConfig& config,
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

/*
  Tries to find the best server for the given hostname, case insensitive. If no
  hostname matches it defaults to the first server that is associated with this
  socket.
*/
const Server* ServerManager::getServerByHost(const Socket* socket,
                                             const std::string& host) const
{
  const std::string lowerHost = ft::to_lower(host);
  const const_SockToServIter iter = _socketToServers.find(socket);
  assert(iter != _socketToServers.end());
  const std::vector<const Server*>& servers = iter->second;
  for (std::size_t i = 0; i < servers.size(); ++i) {
    const std::vector<std::string>& serverNames = servers[i]->getHostnames();
    if (ft::contains(serverNames, lowerHost)) {
      return servers[i];
    }
  }
  assert(!servers.empty());
  return servers[0];
}

/*
  It should never happen that no server is found for this socket.
  However, if the socket is associated with multiple servers, we can only
  return a specific server once we parse the Host header.
  Until then, return nullptr to indicate client is not associated with a
  specific server yet.
*/
const Server* ServerManager::getInitServer(const Socket& socket) const
{
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
  ChildProcessManager& childProcessManager = ChildProcessManager::getInstance();
  EventManager& eventManager = EventManager::getInstance();
  g_running = 1;
  while (g_running == 1) {
    const int res = eventManager.check();
    if (res == 0) {
      _log.info() << "poll: timeout\n";
    }
    if (res < 0) {
      _log.error() << "poll error: " << std::strerror(errno) << '\n';
    }
    eventManager.checkTimeouts();
    childProcessManager.collectChildren();
  }
  _log.info() << "Shutting down servers...\n";
}
