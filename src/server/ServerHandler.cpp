#include "ServerHandler.hpp"
#include "config/Config.hpp"
#include "config/ServerConfig.hpp"
#include "server/Server.hpp"
#include "socket/AutoFd.hpp"
#include "socket/Socket.hpp"
#include <cerrno>
#include <csignal>
#include <cstring>
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>
#include <vector>

static volatile sig_atomic_t g_running = 0;

static void error(const std::string& msg)
{
  std::cerr << "Error: " << msg << " (" << strerror(errno) << ")\n";
}

extern "C" void sigIntHandler(int /*sigNum*/)
{
  g_running = 0;
}

ServerHandler::ServerHandler(const Config& config)
{
  if (signal(SIGINT, sigIntHandler) == SIG_ERR) {
    error("Failed to set SIGINT handler");
    return;
  }
  createServers(config);
}

ServerHandler::~ServerHandler()
{
  for (size_t i = 0; i < _servers.size(); ++i) {
    delete _servers[i];
  }

  for (portToSocketIter it = _portToSocket.begin(); it != _portToSocket.end();
       ++it) {
    delete it->second;
  }
}

void ServerHandler::createServers(const Config& config)
{
  _servers.reserve(config.getServers().size());
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

  addToPfd(sock->getFd());

  return sock;
}

void ServerHandler::addToPfd(int sockFd)
{
  struct pollfd pfd = {};
  pfd.fd = sockFd;
  pfd.events = POLLIN;
  pfd.revents = 0;
  _pfds.push_back(pfd);
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
              << " | Fd: " << it->second->getFd() << " | Ptr:" << it->second
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

// TODO: add client to correct server
void ServerHandler::acceptClient(int sockFd)
{
  const AutoFd clientFd(accept(sockFd, NULL, NULL));
  if (clientFd.get() < 0) {
    error("failed to accept new client");
    return;
  }

  try {
    Socket::setFlags(clientFd.get());
  } catch (std::exception& e) {
    error("failed to accept new client");
    return;
  }

  addToPfd(clientFd.get());
  std::cout << "[SERVER] new client connected, fd=" << clientFd.get() << '\n';
}

bool ServerHandler::disconnectClient(Client& client, std::size_t& idx)
{
  std::cout << "[SERVER] Client " << idx << " disconnected\n";
  close(client.getFd());
  _pfds.erase(_pfds.begin() + static_cast<long>(idx));
  _clients.erase(_clients.begin() + static_cast<long>(idx - 1));
  return false;
}

bool ServerHandler::receiveFromClient(Client& client, std::size_t& idx)
{
  Buffer buffer(MAX_CHUNK);
  const ssize_t bytes = recv(client.getFd(), &buffer[0], buffer.size(), 0);
  if (bytes > 0) {
    client.addToInBuff(buffer);
    // This is just for debugging atm
    std::cout << "Client " << idx << ": ";
    // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
    std::cout.write(reinterpret_cast<const char*>(&buffer[0]),
                    static_cast<std::streamsize>(bytes));
    // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

    // TODO: STATEMACHINE/PARSING

    if (client.hasDataToSend()) {
      _pfds[idx].events = static_cast<short>(
        static_cast<unsigned>(_pfds[idx].events) |
        static_cast<unsigned>(POLLOUT)); // enable POLLOUT safely
    }
  } else if (bytes == 0) {
    return disconnectClient(client, idx);
  } else // bytes < 0
  {
    error("recv failed, removing client");
    return disconnectClient(client, idx);
  }
  return true;
}

bool ServerHandler::sendToClient(Client& client, std::size_t& idx)
{
  const std::size_t maxChunk = MAX_CHUNK;
  const std::size_t toSend = std::min(client.getOutBuff().size(), maxChunk);
  const ssize_t bytes =
    send(client.getFd(), client.getOutBuff().data(), toSend, 0);
  if (bytes > 0) {
    client.removeFromOutBuff(bytes);
    if (!client.hasDataToSend()) {
      _pfds[idx].events =
        static_cast<short>(static_cast<unsigned>(_pfds[idx].events) &
                           ~static_cast<unsigned>(POLLOUT));
    }
  } else if (bytes == 0) {
    std::cout << "[SERVER] no data sent to client " << idx << "\n";
  } else {
    std::cerr << "[SERVER] send error for client " << idx << ": "
              << strerror(errno) << "\n";
    return disconnectClient(client, idx);
  }
  return true;
}

bool ServerHandler::isListener(int sockFd)
{
  for (portToSocketIter it = _portToSocket.begin(); it != _portToSocket.end();
       ++it) {
    if (sockFd == it->second->getFd()) {
      return true;
    }
  }
  return false;
}

void ServerHandler::checkActivity()
{
  for (std::size_t i = 0; i < _pfds.size();) {
    bool same = true;
    const unsigned events = static_cast<unsigned>(_pfds[i].revents);
    if (isListener(_pfds[i].fd)) {
      if ((events & POLLIN) != 0) {
        acceptClient();
      }
    } else {
      Client& client = _clients[i - 1];
      if ((events & POLLIN) != 0 && same) { // Receive Data
        same = receiveFromClient(client, i);
      }
      if ((events & POLLOUT) != 0 && same) { // Send Data
        same = sendToClient(client, i);
      }
      if ((events & static_cast<unsigned>(POLLHUP | POLLERR)) != 0 &&
          same) { // Error
        same = disconnectClient(client, i);
      }
    }
    if (same) {
      i++;
    }
  }
}

void ServerHandler::run()
{
  g_running = 1;
  while (g_running != 0) {
    // TODO: calculate timeout
    const int ready = poll((&_pfds[0]), _pfds.size(), -1);
    if (ready < 0) {
      if (errno == EINTR) {
        continue;
      }
      error("poll failed");
      break;
    }
    if (ready == 0) {
      std::cerr << "Error: poll timeout\n";
      // TODO: add cleanup and reset timeout, only break when no servers left
      break;
    }
    checkActivity();
  }
  std::cout << "Shutting down servers...\n";
}
