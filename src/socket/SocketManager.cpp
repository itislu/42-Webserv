#include "SocketManager.hpp"
#include "Socket.hpp"
#include "config/Config.hpp"
#include "config/ServerConfig.hpp"
#include <cstddef>
#include <exception>
#include <map>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>
#include <vector>

// Owns _portToSocket and _pfds.
// Handles creating listeners and pollfds.

SocketManager::SocketManager(const Config* config)
{
  createListeningSockets(config->getServers());
}

SocketManager::~SocketManager()
{
  for (sockIter it = _sockets.begin(); it != _sockets.end(); ++it) {
    delete (*it);
  }
}

void SocketManager::createListeningSockets(
  const std::vector<ServerConfig>& configs)
{
  for (std::vector<ServerConfig>::const_iterator it = configs.begin();
       it != configs.end();
       ++it) {
    createListener((*it).getPorts());
  }
}

void SocketManager::createListener(const std::vector<int>& ports)
{
  for (std::vector<int>::const_iterator it = ports.begin(); it != ports.end();
       ++it) {
    if (listenerExists(*it)) {
      continue;
    }
    const Socket* const socket = new Socket(*it);
    _sockets.push_back(socket);
    addToFdSocketMap(socket->getFd(), socket);
    addToPfd(socket->getFd());
  }
}

bool SocketManager::listenerExists(int port) const
{
  for (std::map<int, const Socket*>::const_iterator it = _fdToSocket.begin();
       it != _fdToSocket.end();
       ++it) {
    if ((*it).second->getPort() == port) {
      return true;
    }
  }
  return false;
}

bool SocketManager::isListener(int fdes) const
{
  const std::map<int, const Socket*>::const_iterator iter =
    _fdToSocket.find(fdes);
  return iter != _fdToSocket.end();
}

void SocketManager::addToFdSocketMap(int fdes, const Socket* socket)
{
  _fdToSocket.insert(std::make_pair(fdes, socket));
}

void SocketManager::addToPfd(int fdes)
{
  struct pollfd pfd = {};
  pfd.fd = fdes;
  pfd.events = POLLIN;
  pfd.revents = 0;
  _pfds.push_back(pfd);
}

std::size_t SocketManager::getPfdsSize() const
{
  return _pfds.size();
}

std::vector<pollfd> SocketManager::getPfds() const
{
  return _pfds;
}

pollfd* SocketManager::getPfdStart()
{
  return _pfds.data();
}

int SocketManager::acceptClient(int fdes)
{
  const int clientFd = accept(fdes, NULL, NULL);
  if (clientFd < 0) {

    return -1;
  }

  try {
    Socket::setFlags(clientFd);
    addToPfd(clientFd);
    const Socket* const listener = getSocket(fdes);
    if (listener != NULL) {
      _fdToSocket[clientFd] = listener;
    }
  } catch (std::exception& e) {
    close(clientFd);
    return -1;
  }

  return clientFd;
}

pollfd* SocketManager::getPollFd(int fdes)
{
  for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end();
       ++it) {
    if ((*it).fd == fdes) {
      return &(*it);
    }
  }
  return NULL;
}

void SocketManager::enablePollout(int fdes)
{
  pollfd* const pfd = getPollFd(fdes);
  if (pfd != 0) {
    pfd->events = static_cast<short>(static_cast<unsigned>(pfd->events) |
                                     static_cast<unsigned>(POLLOUT));
  }
}

void SocketManager::disablePollout(int fdes)
{
  pollfd* const pfd = getPollFd(fdes);
  if (pfd != 0) {
    pfd->events = static_cast<short>(static_cast<unsigned>(pfd->events) &
                                     ~static_cast<unsigned>(POLLOUT));
  }
}

const Socket* SocketManager::getSocket(int fdes) const
{
  const std::map<int, const Socket*>::const_iterator iter =
    _fdToSocket.find(fdes);
  if (iter != _fdToSocket.end()) {
    return iter->second;
  }
  return NULL;
}

const Socket* SocketManager::getListener(int port) const
{
  for (const_fdToSockIter it = _fdToSocket.begin(); it != _fdToSocket.end();
       ++it) {
    if ((*it).second->getPort() == port) {
      return it->second;
    }
  }
  return NULL;
}

void SocketManager::removePfd(int fdes)
{
  for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end();
       ++it) {
    if ((*it).fd == fdes) {
      _pfds.erase(it);
      break;
    }
  }
}

void SocketManager::removeFdFromMap(int fdes)
{
  const fdToSockIter iter = _fdToSocket.find(fdes);
  if (iter != _fdToSocket.end()) {
    _fdToSocket.erase(iter);
  }
}

void SocketManager::removeFd(int fdes)
{
  removePfd(fdes);
  removeFdFromMap(fdes);
}
