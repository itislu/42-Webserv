#include "EventManager.hpp"

#include <client/Client.hpp>
#include <client/TimeStamp.hpp>
#include <config/Config.hpp>
#include <event/ClientEventHandler.hpp>
#include <event/EventHandler.hpp>
#include <http/CgiContext.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <server/Server.hpp>
#include <server/ServerManager.hpp>
#include <socket/AutoFd.hpp>
#include <socket/Socket.hpp>
#include <socket/SocketManager.hpp>
#include <stdexcept>
#include <utils/logger/Logger.hpp>

#include <algorithm>
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstring>
#include <exception>
#include <iostream>
#include <sys/poll.h>
#include <vector>

// Handles the main poll loop:
// Calls poll()
// Calls checkActivity() (or delegate to ClientManager/SocketManager)
// Checks timeouts

/* ***************************************************************************/
// INIT

Logger& EventManager::_log = Logger::getInstance(LOG_SERVER);

/* ************************************************************************** */
// PUBLIC

EventManager& EventManager::getInstance()
{
  static EventManager eventManager;

  return eventManager;
}

int EventManager::check()
{
  const int timeout = _calculateTimeout();
  const int ready = poll(
    _socketManager().getPfdStart(), _socketManager().getPfdsSize(), timeout);
  if (ready <= 0) {
    return ready;
  }
  try {
    _checkActivity();
  } catch (const std::exception& e) {
    _log.error() << "Error: " << e.what() << "\n";
  }
  return ready;
}

void EventManager::checkTimeouts()
try {
  std::vector<ft::shared_ptr<EventHandler> > timedOut;
  _getTimedOutHandlers(timedOut);
  for (std::size_t i = 0; i < timedOut.size(); ++i) {
    _log.info() << timedOut[i]->logName() << "timed out.\n";
    _disconnectEventHandler(*timedOut[i]);
  }
} catch (...) {
  // EMPTY: Handle the ones we could collect already.
}

void EventManager::_addHandler(ft::shared_ptr<EventHandler> handler)
{
  const int fdes = handler->getFd();
  assert(fdes > 0);
  _handlers[fdes] = ft::move(handler);
}

EventHandler& EventManager::getHandler(RawFd fdes) const
{
  const const_iterHandler iter = _handlers.find(fdes);
  if (iter == _handlers.end()) {
    throw std::runtime_error("EventManager: fd not found");
  }
  return *iter->second;
}

void EventManager::addCgiHandler(ft::shared_ptr<EventHandler> handler)
{
  _socketManager().addCgiFd(handler->getFd());
  _addHandler(ft::move(handler));
}

/* ************************************************************************** */
// PRIVATE

ServerManager& EventManager::_serverManager()
{
  return ServerManager::getInstance();
}

SocketManager& EventManager::_socketManager()
{
  return SocketManager::getInstance();
}

void EventManager::_checkActivity()
{
  const SocketManager& socketManager = _socketManager();
  const std::vector<pollfd>& pfds = socketManager.getPfds();
  for (std::size_t i = 0; i < pfds.size();) {
    const unsigned events = static_cast<unsigned>(pfds[i].revents);
    if (socketManager.isListener(pfds[i].fd)) {
      _acceptClient(pfds[i].fd, events);
      i++;
    } else {
      EventHandler& handler = getHandler(pfds[i].fd);
      const EventHandler::Result result = handler.handleEvent(events);
      if (result == EventHandler::Disconnect) {
        _disconnectEventHandler(handler);
      } else {
        ++i;
      }
    }
  }
}

void EventManager::_acceptClient(int fdes, const unsigned events)
{
  if ((events & POLLIN) == 0) {
    return;
  }

  AutoFd clientFd = _socketManager().acceptClient(fdes);
  const int clientFdRaw = clientFd.get();
  if (clientFdRaw > 0) {
    const Socket& socket = _socketManager().getSocket(fdes);
    const Server* const server = _serverManager().getInitServer(socket);
    ft::shared_ptr<Client> client(
      new Client(ft::move(clientFd), server, &socket));
    ft::shared_ptr<ClientEventHandler> handler =
      ft::make_shared<ClientEventHandler>(clientFdRaw, ft::move(client));
    _addHandler(ft::move(handler));
    _log.info() << "[SERVER] new client connected, fd=" << clientFdRaw << '\n';
  } else {
    _log.error() << "[SERVER] accepting new client failed\n";
  }
}

void EventManager::_disconnectEventHandler(const EventHandler& handler)
{
  _log.info() << handler.logName() << "disconnected\n";

  const int handlerFd = handler.getFd();

  // Remove corresponding pollfd
  _socketManager().removeFd(handlerFd);

  // remove from handler map
  _removeHandler(handlerFd);
}

int EventManager::_calculateTimeout()
{
  // No clients yet, get default
  if (_handlers.empty()) {
    const long timeout = Config::getDefaultTimeout();
    const int timeoutMs = convertSecondsToMs(timeout);
    // _log.info() << "No clients - use default timeout: " << timeoutMs <<
    // "ms\n";
    return timeoutMs;
  }

  const long minRemaining = _getMinTimeout();
  const int timeoutMs = convertSecondsToMs(minRemaining);
  // _log.info() << "using client min remaining timeout: " << timeoutMs <<
  // "ms\n";

  return timeoutMs;
}

long EventManager::_getMinTimeout() const
{
  const TimeStamp now;
  long minRemaining = LONG_MAX;
  for (const_iterHandler it = _handlers.begin(); it != _handlers.end(); ++it) {
    const long clientTimeout = it->second->getTimeout();
    const long remaining =
      clientTimeout - (now - it->second->getLastActivity());
    minRemaining = std::min(std::max(0L, remaining), minRemaining);
  }
  return minRemaining;
}

void EventManager::_getTimedOutHandlers(
  std::vector<ft::shared_ptr<EventHandler> >& timedOut) const
try {
  const TimeStamp now;
  for (const_iterHandler it = _handlers.begin(); it != _handlers.end(); ++it) {
    EventHandler& handler = *(it->second);
    const long timeout = handler.getTimeout();
    if (now - handler.getLastActivity() >= timeout) {
      const EventHandler::Result result = handler.onTimeout();
      if (result == EventHandler::Disconnect) {
        timedOut.push_back(it->second);
      }
    }
  }
} catch (...) {
  // EMPTY: Handle the ones we could collect already.
}

void EventManager::_removeHandler(RawFd fdes)
{
  const iterHandler iter = _handlers.find(fdes);
  if (iter != _handlers.end()) {
    _handlers.erase(iter);
  }
}
