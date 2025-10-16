#include "ServerHandler.hpp"
#include "client/Client.hpp"
#include "config/Config.hpp"
#include "config/ServerConfig.hpp"
#include "server/Server.hpp"
#include "socket/AutoFd.hpp"
#include "socket/Socket.hpp"
#include "utils/Buffer.hpp"
#include <algorithm>
#include <cerrno>
#include <csignal>
#include <cstddef>
#include <cstring>
#include <exception>
#include <iostream>
#include <map>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
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
// can only be done after parsing Host header in HTTP
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
  _clients.push_back(new Client(clientFd.get()));

  std::cout << "[SERVER] new client connected, fd=" << clientFd.get() << '\n';
}

void ServerHandler::disconnectClient(Client* client, const std::size_t idx)
{
  std::cout << "[SERVER] Client fd=" << client->getFd() << " disconnected\n";

  // remove pfd for client
  _pfds.erase(_pfds.begin() + static_cast<long>(idx));

  // remove from client vector
  if (client != 0) {
    for (std::vector<Client*>::iterator it = _clients.begin();
         it != _clients.end();
         ++it) {
      if (*it == client) {
        delete *it;
        _clients.erase(it);
        break;
      }
    }
  }
}

pollfd* ServerHandler::getPollFdForClient(Client* client)
{
  for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end();
       ++it) {
    if (it->fd == client->getFd()) {
      return &(*it);
    }
  }
  return NULL;
}

bool ServerHandler::receiveFromClient(Client* client)
{
  const Buffer buffer(MAX_CHUNK);
  const ssize_t bytes =
    recv(client->getFd(), buffer.data(), buffer.getSize(), 0);
  if (bytes > 0) {
    client->getInBuff().add(buffer);
    // This is just for debugging atm
    std::cout << "Client " << client->getFd() << ": ";
    // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
    std::cout.write(reinterpret_cast<const char*>(buffer.data()),
                    static_cast<std::streamsize>(bytes));
    // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

    // TODO: STATEMACHINE/PARSING

    if (client->hasDataToSend()) {
      pollfd* const pfd = getPollFdForClient(client);
      pfd->events = static_cast<short>(
        static_cast<unsigned>(pfd->events) |
        static_cast<unsigned>(POLLOUT)); // enable POLLOUT safely
    }
  } else if (bytes == 0) {
    return false;
  } else // bytes < 0
  {
    error("recv failed, removing client");
    return false;
  }
  return true;
}

bool ServerHandler::sendToClient(Client* client)
{
  const std::size_t toSend = std::min(client->getOutBuff().getSize(),
                                      static_cast<std::size_t>(MAX_CHUNK));
  const ssize_t bytes =
    send(client->getFd(), client->getOutBuff().data(), toSend, 0);

  if (bytes > 0) {
    client->getOutBuff().remove(bytes);
    if (!client->hasDataToSend()) {
      if (pollfd* const pfd = getPollFdForClient(client)) {
        pfd->events =
          static_cast<short>(static_cast<unsigned>(pfd->events) &
                             ~static_cast<unsigned>(
                               POLLOUT)); // disable write when buffer is empty
      }
    }
  } else if (bytes == 0) {
    std::cout << "[SERVER] no data sent to client fd=" << client->getFd()
              << "\n";
  } else {
    std::cerr << "[SERVER] send error for client fd=" << client->getFd() << ": "
              << strerror(errno) << "\n";
    return false;
  }
  return true;
}

bool ServerHandler::handleClient(Client* client, const unsigned events)
{
  if (client == 0) {
    return false;
  }
  bool alive = true;
  if ((events & POLLIN) != 0 && alive) { // Receive Data
    alive = receiveFromClient(client);
  }
  if ((events & POLLOUT) != 0 && alive) { // Send Data
    alive = sendToClient(client);
  }
  if ((events & static_cast<unsigned>(POLLHUP | POLLERR)) != 0 && alive) {
    return false; // disconnect client
  }
  return alive;
}

Client* ServerHandler::getClientFromFd(int clientFd)
{
  for (std::vector<Client*>::iterator it = _clients.begin();
       it != _clients.end();
       ++it) {
    if ((*it)->getFd() == clientFd) {
      return *it;
    }
  }
  return NULL;
}

void ServerHandler::checkActivity()
{
  for (std::size_t i = 0; i < _pfds.size();) {
    const unsigned events = static_cast<unsigned>(_pfds[i].revents);
    if (isListener(_pfds[i].fd)) {
      if ((events & POLLIN) != 0) {
        acceptClient(_pfds[i].fd);
        i++;
      }
    } else {
      Client* const client = getClientFromFd(_pfds[i].fd);
      if (!handleClient(client, events)) {
        disconnectClient(client, i);
      } else {
        i++;
      }
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

/* bool ServerHandler::disconnectClient(std::size_t& idx)


bool ServerHandler::receiveFromClient(Client* client)
{
  Buffer buffer(MAX_CHUNK);
  const ssize_t bytes =
    recv(client.getFd(), buffer.data(), buffer.getSize(), 0);
  if (bytes > 0) {
    client.addToInBuff(buffer);
    // This is just for debugging atm
    std::cout << "Client " << client.getFd() << ": ";
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
    return false;
  } else // bytes < 0
  {
    error("recv failed, removing client");
    return false;
  }
  return true;
}

bool ServerHandler::sendToClient(Client* client)
{
  // const std::size_t maxChunk = MAX_CHUNK;
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


} */
