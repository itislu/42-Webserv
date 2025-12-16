#include "ServerManager.hpp"

#include <config/Config.hpp>
#include <config/ServerConfig.hpp>
#include <event/EventManager.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <server/Server.hpp>
#include <socket/Socket.hpp>
#include <socket/SocketManager.hpp>
#include <utils/process/ChildProcessManager.hpp>

#include <cassert>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/signal.h>
#include <vector>

static volatile std::sig_atomic_t g_running = 0;
static volatile std::sig_atomic_t g_childDied = 0;

static void error(const std::string& msg)
{
  std::cerr << "Error: " << msg << " (" << std::strerror(errno) << ")\n";
}

extern "C" void sigIntHandler(int /*sigNum*/)
{
  g_running = 0;
}

extern "C" void sigChldHandler(int /*sigNum*/)
{
  g_childDied = 1;
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
  if (std::signal(SIGCHLD, sigChldHandler) == SIG_ERR) {
    throw std::runtime_error("Failed to set SIGCHLD handler");
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
  Tries to find the best server for the given hostname. If no hostname matches
  it defaults to the first server that is associated with this socket.
*/
const Server* ServerManager::getServerByHost(const Socket* socket,
                                             const std::string& host) const
{
  const const_SockToServIter iter = _socketToServers.find(socket);
  if (iter == _socketToServers.end()) {
    return FT_NULLPTR; // this should never happen
  }
  const std::vector<const Server*>& servers = iter->second;
  for (std::size_t i = 0; i < servers.size(); ++i) {
    const std::vector<std::string>& serverNames = servers[i]->getHostnames();
    for (std::size_t name = 0; name < serverNames.size(); ++name) {
      if (host == serverNames[name]) {
        return servers[i];
      }
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
      std::cout << "poll: timeout\n";
    }
    if (res < 0) {
      if (errno == EINTR) {
        // ok interupted by singnal;
      } else {
        error("poll: failed");
        std::cerr << "exit: poll failed\n";
        break;
      }
    }
    eventManager.checkTimeouts();
    if (g_childDied == 1) {
      g_childDied = 0;
      childProcessManager.collectChilds();
    }
  }
  std::cout << "Shutting down servers...\n";
}
