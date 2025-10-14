#include "ServerHandler.hpp"
#include "config/Config.hpp"
#include "config/ServerConfig.hpp"
#include "server/Server.hpp"
#include "socket/Socket.hpp"
#include <cerrno>
#include <csignal>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <utility>
#include <vector>

// static volatile sig_atomic_t g_running = 0;

ServerHandler::ServerHandler(const Config& config)
{
  createServers(config);
}

void ServerHandler::createServers(const Config& config)
{
  for (std::vector<ServerConfig>::const_iterator it =
         config.getServers().begin();
       it != config.getServers().end();
       ++it) {

    const std::vector<const Socket*> currListeners =
      createListeners(it->getPorts());

    _servers.push_back(new Server(*it, currListeners));
    mapServerToListeners(it->getPorts(), _servers.back());
  }
}

std::vector<const Socket*> ServerHandler::createListeners(
  const std::vector<int>& ports)
{
  std::vector<const Socket*> listeners;
  listeners.reserve(ports.size());
  for (std::vector<int>::const_iterator it = ports.begin(); it != ports.end();
       ++it) {
    const int port = *it;
    const Socket* const sock = getListener(port);
    listeners.push_back(sock);
  }
  return listeners;
}

const Socket* ServerHandler::getListener(int port)
{
  const portToSocketIter iter = _portToSocket.find(port);
  if (iter != _portToSocket.end()) {
    return iter->second;
  }
  const Socket* const sock = new Socket(port);
  _portToSocket.insert(std::pair<int, const Socket*>(port, sock));
  return sock;
}

void ServerHandler::mapServerToListeners(const std::vector<int>& ports,
                                         Server* server)
{
  for (std::vector<int>::const_iterator it = ports.begin(); it != ports.end();
       ++it) {
    const int port = *it;
    const Socket* const keySocket = getListener(port);
    _socketToServers[keySocket].push_back(server);
  }
}

void ServerHandler::debugPrintMaps() const
{
  std::cout << "\n============ Ports/Sockets ============\n";
  for (portToSocketIter it = _portToSocket.begin(); it != _portToSocket.end();
       ++it) {
    std::cout << "Port: " << it->first << "\n"
              << "Socket: " << it->second->getPort()
              << " | Fd: " << it->second->getRawFd() << " | Ptr:" << it->second
              << "\n\n";
  }
  std::cout << "=========== Sockets/Servers ===========\n";
  for (socketToServersIter it = _socketToServers.begin();
       it != _socketToServers.end();
       ++it) {
    std::cout << "Socket: " << it->first << "\n";
    for (std::vector<Server*>::const_iterator servIt = it->second.begin();
         servIt != it->second.end();
         ++servIt) {
      std::cout << "└── Server: " << *servIt << "\n";
      std::cout << "    " << (*servIt)->getHostnames().size() << ".Hosts: ";
      for (std::vector<std::string>::const_iterator hostIt =
             (*servIt)->getHostnames().begin();
           hostIt != (*servIt)->getHostnames().end();
           ++hostIt) {
        std::cout << *hostIt;
        std::vector<std::string>::const_iterator next = hostIt;
        if (++next != (*servIt)->getHostnames().end()) {
          std::cout << " | ";
        }
      }
      std::cout << "\n";
    }
    std::cout << "\n";
  }
  std::cout << "=======================================\n\n";
}

void ServerHandler::init()
{
  
}

// void ServerHandler::addToPfd(int sockFd)
// {
//   struct pollfd pfd = {};
//   pfd.fd = sockFd;
//   pfd.events = POLLIN;
//   pfd.revents = 0;
//   _pfds.push_back(pfd);
// }

// void ServerHandler::run()
// {
//   g_running = 1;
//   while (g_running != 0) {
//     // TODO: calculate timeout
//     const int ready = poll((&_pfds[0]), _pfds.size(), -1);
//     if (ready < 0) {
//       if (errno == EINTR) {
//         continue;
//       }
//       // error("poll failed");
//       break;
//     }
//     if (ready == 0) {
//       std::cerr << "Error: poll timeout\n";
//       break;
//     }
//     // checkActivity();
//   }
//   std::cout << "Shutting down servers...\n";
// }
