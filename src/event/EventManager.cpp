#include "EventManager.hpp"
#include "client/Client.hpp"
#include "client/ClientManager.hpp"
#include "config/Config.hpp"
#include "socket/SocketManager.hpp"
#include <algorithm>
#include <climits>
#include <csignal>
#include <cstring>
#include <iostream>
#include <sys/poll.h>
#include <vector>

// Handles the main poll loop:
// Calls poll()
// Calls checkActivity() (or delegate to ClientManager/SocketManager)
// Checks timeouts

EventManager::EventManager(ClientManager* clients, SocketManager* sockets)
  : _clientsManager(clients)
  , _socketsManager(sockets)
{
}

bool EventManager::sendToClient(Client* client)
{
  const bool alive = client->sendTo();
  if (alive && !client->hasDataToSend()) {
    _socketsManager->disablePollout(client->getFd());
  }
  return alive;
}

bool EventManager::receiveFromClient(Client* client)
{
  const bool alive = client->receive();
  if (alive && client->hasDataToSend()) {
    _socketsManager->enablePollout(client->getFd());
  }
  return alive;
}

bool EventManager::handleClient(Client* client, unsigned events)
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

void EventManager::disconnectClient(Client* client)
{
  if (client == 0) {
    return;
  }

  const int clientFd = client->getFd();
  // Remove corresponding pollfd
  _socketsManager->removeFd(clientFd);

  // remove from client map
  _clientsManager->removeClient(clientFd);

  std::cout << "[SERVER] Client fd=" << clientFd << " disconnected\n";
}

void EventManager::acceptClient(int fdes, const unsigned events)
{
  if ((events & POLLIN) != 0) {
    if (_socketsManager->acceptClient(fdes)) {
      _clientsManager->addClient(fdes);
    }
  }
}

void EventManager::checkActivity()
{
  std::vector<pollfd> pfds = _socketsManager->getPfds();
  for (std::size_t i = 0; i < pfds.size();) {
    const unsigned events = static_cast<unsigned>(pfds[i].revents);
    if (_socketsManager->isListener(pfds[i].fd)) {
      acceptClient(pfds[i].fd, events);
      i++;
    } else {
      Client* const client = _clientsManager->getClient(pfds[i].fd);
      if (!handleClient(client, events)) {
        disconnectClient(client);
      } else {
        i++;
      }
    }
  }
}

int EventManager::calculateTimeout() const
{
  // No clients yet, get default
  if (!_clientsManager->hasClients()) {
    long timeout = Config::getDefaultTimeout() * MS_MULTIPLIER;
    timeout = std::min(timeout, static_cast<long>(INT_MAX));
    timeout = std::max(timeout, 0L);
    return static_cast<int>(timeout);
  }

  const long minRemaining = _clientsManager->getMinTimeout();
  long timeoutMs = minRemaining * MS_MULTIPLIER;
  timeoutMs = std::min(timeoutMs, static_cast<long>(INT_MAX));
  timeoutMs = std::max(timeoutMs, 0L);

  std::cout << "current timeout time: " << timeoutMs << "ms\n";

  return static_cast<int>(timeoutMs);
}

void EventManager::checkTimeouts()
{
  std::vector<Client*> timedOut;
  _clientsManager->getTimedOutClients(timedOut);
  for (size_t i = 0; i < timedOut.size(); ++i) {
    std::cout << "[SERVER] Client fd=" << timedOut[i]->getFd()
              << " timed out.\n";
    disconnectClient(timedOut[i]);
  }
}

int EventManager::check()
{
  const int timeout = calculateTimeout();
  const int ready = poll(
    _socketsManager->getPfdStart(), _socketsManager->getPfdsSize(), timeout);
  if (ready <= 0) {
    return ready;
  }
  checkActivity();
  checkTimeouts();
  return ready;
}
