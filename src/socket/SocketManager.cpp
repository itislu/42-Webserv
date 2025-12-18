#include "SocketManager.hpp"

#include "AutoFd.hpp"
#include "Socket.hpp"
#include <config/Config.hpp>
#include <config/ServerConfig.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>

#include <cassert>
#include <cstddef>
#include <sys/poll.h>
#include <sys/socket.h>
#include <utility>
#include <vector>

// Owns _portToSocket and _pfds.
// Handles creating listeners and pollfds.

SocketManager::SocketManager(const Config& config)
{
  createListeningSockets(config.getServers());
}

SocketManager& SocketManager::getInstance()
{
  static SocketManager socketManager(Config::getConfig());

  return socketManager;
}

// only call in the constructor - because no exception guarantee
void SocketManager::createListeningSockets(
  const std::vector<ServerConfig>& configs)
{
  for (Config::const_ServConfIter it = configs.begin(); it != configs.end();
       ++it) {
    createListener(it->getPorts());
  }
}

// only call in the constructor - because no exception guarantee
void SocketManager::createListener(const std::vector<int>& ports)
{
  for (std::vector<int>::const_iterator it = ports.begin(); it != ports.end();
       ++it) {
    if (listenerExists(*it)) {
      continue;
    }
    const ft::shared_ptr<const Socket> socket =
      ft::make_shared<const Socket>(*it);
    _sockets.push_back(socket);
    _listeners.insert(std::make_pair(socket->getFd(), socket.get()));
    addToPfd(socket->getFd());
  }
}

bool SocketManager::listenerExists(int port) const
{
  for (const_FdToSockIter it = _listeners.begin(); it != _listeners.end();
       ++it) {
    if (it->second->getPort() == port) {
      return true;
    }
  }
  return false;
}

bool SocketManager::isListener(int fdes) const
{
  const const_FdToSockIter iter = _listeners.find(fdes);
  return iter != _listeners.end();
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

const std::vector<pollfd>& SocketManager::getPfds() const
{
  return _pfds;
}

pollfd* SocketManager::getPfdStart()
{
  return _pfds.data();
}

AutoFd SocketManager::acceptClient(int fdes, sockaddr_storage& addr)
{
  socklen_t addrLen = sizeof(addr);
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  AutoFd clientFd(accept(fdes, reinterpret_cast<sockaddr*>(&addr), &addrLen));
  if (clientFd.get() < 0) {
    return ft::move(clientFd);
  }
  clientFd.subscribe(*this);

  Socket::setFlags(clientFd.get());
  addToPfd(clientFd.get());
  const Socket& listener = getSocket(fdes);
  _fdToSocket[clientFd.get()] = &listener;

  return ft::move(clientFd);
}

void SocketManager::addCgiFd(int fdes)
{
  addToPfd(fdes);
}

pollfd* SocketManager::getPollFd(int fdes)
{
  for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end();
       ++it) {
    if (it->fd == fdes) {
      return &(*it);
    }
  }
  return FT_NULLPTR;
}

void SocketManager::enablePollout(int fdes)
{
  pollfd* const pfd = getPollFd(fdes);
  if (pfd != FT_NULLPTR) {
    pfd->events = static_cast<short>(static_cast<unsigned>(pfd->events) |
                                     static_cast<unsigned>(POLLOUT));
  }
}

void SocketManager::disablePollout(int fdes)
{
  pollfd* const pfd = getPollFd(fdes);
  if (pfd != FT_NULLPTR) {
    pfd->events = static_cast<short>(static_cast<unsigned>(pfd->events) &
                                     ~static_cast<unsigned>(POLLOUT));
  }
}

const Socket& SocketManager::getSocket(int fdes) const
{
  const const_FdToSockIter iter = _listeners.find(fdes);
  assert(iter != _listeners.end() && "SocketManager::getSocket: fd not found");
  return *iter->second;
}

const Socket& SocketManager::getListener(int port) const
{
  for (const_FdToSockIter it = _listeners.begin(); it != _listeners.end();
       ++it) {
    if (it->second->getPort() == port) {
      return *it->second;
    }
  }
  assert(false && "SocketManager::getListener: port not found");
  FT_UNREACHABLE();
}

void SocketManager::removePfd(int fdes)
{
  for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end();
       ++it) {
    if (it->fd == fdes) {
      *it = _pfds.back();
      _pfds.pop_back();
      break;
    }
  }
}

void SocketManager::removeFdFromMap(int fdes)
{
  const FdToSockIter iter = _fdToSocket.find(fdes);
  if (iter != _fdToSocket.end()) {
    _fdToSocket.erase(iter);
  }
}

void SocketManager::removeFd(int fdes)
{
  removePfd(fdes);
  removeFdFromMap(fdes);
}

void SocketManager::onClose(int fdes)
{
  removeFd(fdes);
}
