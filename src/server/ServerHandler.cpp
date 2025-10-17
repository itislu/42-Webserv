#include "ServerHandler.hpp"
#include "client/Client.hpp"
#include "config/Config.hpp"
#include "config/ServerConfig.hpp"
#include "server/Server.hpp"
#include "socket/AutoFd.hpp"
#include "socket/Socket.hpp"
#include "utils/Buffer.hpp"
#include "utils/TimeStamp.hpp"
#include <algorithm>
#include <cerrno>
#include <climits>
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
  : _config(&config)
{
  if (signal(SIGINT, sigIntHandler) == SIG_ERR) {
    error("Failed to set SIGINT handler");
    return;
  }
  _lowestTimeOut = config.getDefaultTimeout();
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

void ServerHandler::addToPfd(int fdes)
{
  struct pollfd pfd = {};
  pfd.fd = fdes;
  pfd.events = POLLIN;
  pfd.revents = 0;
  _pfds.push_back(pfd);
}

void ServerHandler::addToClients(int sockFd, int clientFd)
{
  const Socket* const sock = getSocketFromFd(sockFd);
  _clients.push_back(new Client(clientFd, sock, getServerFromSocket(sock)));
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

const Socket* ServerHandler::getSocketFromFd(int sockFd)
{
  for (socketToServersIter it = _socketToServers.begin();
       it != _socketToServers.end();
       ++it) {
    if (it->first->getFd() == sockFd) {
      return it->first;
    }
  }
  return NULL;
}

const Server* ServerHandler::getServerFromSocket(const Socket* socket)
{
  const std::map<const Socket*, std::vector<Server*> >::const_iterator iter =
    _socketToServers.find(socket);
  if (iter == _socketToServers.end()) {
    std::cout << "No server found for this socket\n";
    return NULL;
  }

  const std::vector<Server*>& servers = iter->second;
  if (servers.size() != 1) {
    std::cout << "Found multiple servers, don't assign any: " << servers.size()
              << "\n";
    return NULL;
  }

  std::cout << "Found only one server, assign it instantly to client\n";
  return servers[0];
}

void ServerHandler::acceptClient(int sockFd)
{
  const int clientFd = accept(sockFd, NULL, NULL);
  if (clientFd < 0) {
    error("failed to accept new client");
    return;
  }

  try {
    Socket::setFlags(clientFd);
  } catch (std::exception& e) {
    error("failed to accept new client");
    return;
  }

  addToPfd(clientFd);
  addToClients(sockFd, clientFd);

  std::cout << "[SERVER] new client connected, fd=" << clientFd << '\n';
}

void ServerHandler::disconnectClient(Client* client)
{
  if (client == 0) {
    return;
  }

  const int clientFd = client->getFd();

  // Remove corresponding pollfd
  for (pfdIter pfdIt = _pfds.begin(); pfdIt != _pfds.end(); ++pfdIt) {
    if (pfdIt->fd == clientFd) {
      _pfds.erase(pfdIt);
      break;
    }
  }

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
  std::cout << "[SERVER] Client fd=" << clientFd << " disconnected\n";
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
  std::vector<unsigned char> buffer(MAX_CHUNK);
  const ssize_t bytes = recv(client->getFd(), buffer.data(), buffer.size(), 0);
  if (bytes > 0) {
    // This is just for debugging atm
    std::cout << "Client " << client->getFd() << ": ";
    // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
    std::cout.write(reinterpret_cast<const char*>(buffer.data()),
                    static_cast<std::streamsize>(bytes));
    // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
    client->getInBuff().add(buffer);

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
  client->updateLastActivity();
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
  client->updateLastActivity();
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
      }
      i++;
    } else {
      Client* const client = getClientFromFd(_pfds[i].fd);
      if (!handleClient(client, events)) {
        disconnectClient(client);
      } else {
        i++;
      }
    }
  }
}

long ServerHandler::getClientTimeOut(const Client* const client) const
{
  if (client->getServer() != 0) {
    return client->getServer()->getTimeout();
  }
  return _config->getDefaultTimeout();
}

int ServerHandler::calculateTimeOut() const
{
  if (_clients.empty()) {
    long timeoutMs = _lowestTimeOut * MS_MULTIPLIER;
    timeoutMs = std::min(timeoutMs, static_cast<long>(INT_MAX));
    timeoutMs = std::max(timeoutMs, 0L);
    std::cout << "No clients, default timeout: " << timeoutMs << "ms\n";
    return static_cast<int>(timeoutMs);
  }

  const TimeStamp now;
  long minRemaining = LONG_MAX;

  for (clientIter it = _clients.begin(); it != _clients.end(); ++it) {
    const long clientTimeout = getClientTimeOut(*it);
    const long remaining = clientTimeout - (now - (*it)->getLastActivity());
    minRemaining = std::min(remaining, minRemaining);
  }

  long timeoutMs = minRemaining * MS_MULTIPLIER;
  timeoutMs = std::min(timeoutMs, static_cast<long>(INT_MAX));
  timeoutMs = std::max(timeoutMs, 0L);

  std::cout << "current timeout time: " << timeoutMs << "ms\n";

  return static_cast<int>(timeoutMs);
}

void ServerHandler::checkClientTimeouts()
{
  const TimeStamp now;
  for (clientIter it = _clients.begin(); it != _clients.end();) {
    const long timeOut = getClientTimeOut(*it);
    if (now - (*it)->getLastActivity() >= timeOut) {
      std::cout << "[SERVER] Client fd=" << (*it)->getFd() << " timed out.\n";
      disconnectClient(*it);
    } else {
      ++it;
    }
  }
}

void ServerHandler::run()
{
  g_running = 1;
  while (g_running != 0) {
    const int timeOut = calculateTimeOut();
    const int ready = poll((&_pfds[0]), _pfds.size(), timeOut);
    if (ready < 0) {
      if (errno == EINTR) {
        continue;
      }
      error("poll failed");
      break;
    }
    if (ready == 0) {
      // std::cout << "poll timeout: " << TimeStamp::now() << "\n";
    }
    if (ready > 0) {
      checkActivity();
    }
    checkClientTimeouts();
  }
  std::cout << "Shutting down servers...\n";
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
