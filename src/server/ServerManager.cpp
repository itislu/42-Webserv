#include "ServerManager.hpp"
#include "config/Config.hpp"
#include "config/ServerConfig.hpp"
#include "server/Server.hpp"
#include "socket/Socket.hpp"
#include <iostream>
#include <csignal>
#include <string>
#include <iostream>
#include <cstring>

// Owns _servers and _config.
// Creates servers and maps them to sockets.
// Responsible for server-related queries like getServerFromSocket().

typedef const std::vector<ServerConfig>::const_iterator c_srvConfIter;

static volatile sig_atomic_t g_running = 0;

static void error(const std::string& msg)
{
  std::cerr << "Error: " << msg << " (" << strerror(errno) << ")\n";
}

extern "C" void sigIntHandler(int /*sigNum*/)
{
  g_running = 0;
}

ServerManager::ServerManager(const Config& config) 
: _config(config),
 _socketManager(config), 
 _clientManager(),
 _eventManager(_clientManager, _socketManager)
{
  if (signal(SIGINT, sigIntHandler) == SIG_ERR) {
		error("Failed to set SIGINT handler");
		return;
  }
  createServers(config.getServers());
}

ServerManager::~ServerManager() 
{
	for (size_t i = 0; i < _servers.size(); ++i)
	{
		delete _servers[i];
	}
}

void ServerManager::createServers(const std::vector<ServerConfig>& configs)
{
  _servers.reserve(configs.size());
  for (std::vector<ServerConfig>::const_iterator it = configs.begin(); it != configs.end(); ++it)
  {
    const std::vector<const Socket*> listeners = createListeners((*it).getPorts());
    addServer(*it, listeners);
  }
}

std::vector<const Socket*> ServerManager::createListeners(const std::vector<int>& ports)
{
  std::vector<const Socket*> listeners;
  listeners.reserve(ports.size());
  for (std::vector<int>::const_iterator it = ports.begin(); it != ports.end();
       ++it) {
    const int port = *it;
    const Socket* const sock = _socketManager.getListener(port);
    listeners.push_back(sock);
  }
  return listeners;
}

void ServerManager::addServer(const ServerConfig& config, const std::vector<const Socket*>& listeners)
{
  Server* server = new Server(config, listeners);
  _servers.push_back(server);
  mapServerToSocket(server, listeners);
}

void ServerManager::mapServerToSocket(const Server* server, const std::vector<const Socket*>& listeners)
{
  // Map socket pointers -> servers (multiple servers can share a socket)
  for (std::vector<const Socket*>::const_iterator it = listeners.begin();
       it != listeners.end();
       ++it) {
    _socketToServers[*it].push_back(server);
  }
}

const Server* ServerManager::getServerFromSocket(const Socket* socket) const
{
  if (socket == NULL){
    return NULL;
  }
  c_sockToServIter it = _socketToServers.find(socket);
  if (it == _socketToServers.end()){
    return NULL;
  }
  const std::vector<const Server*>& servers = it->second;
  if (servers.size() != 1) {
    return NULL;
  }
  return servers[0];
}

size_t ServerManager::serverCount() const
{
  return _servers.size();
}

const std::vector<Server*>& ServerManager::getServers()
{
  return _servers;
}

void ServerManager::run()
{
  g_running = 1;
  while(g_running == 1)
  {
    int res = _eventManager.check();
    if (res == 0)
      std::cout << "Poll: timeout\n";
    if (res < 0)
    {
      if (errno != EINTR)
        error("poll: failed");
      break;
    }
    checkClientTimeouts();
  }
  std::cout << "Shutting down servers...\n";
}