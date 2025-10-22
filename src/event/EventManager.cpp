#include "EventManager.hpp"
#include "client/Client.hpp"
#include "client/ClientManager.hpp"
#include "client/TimeStamp.hpp"
#include "config/Config.hpp"
#include "server/Server.hpp"
#include "server/ServerManager.hpp"
#include "socket/SocketManager.hpp"
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

EventManager::EventManager(ClientManager* clients,
                           SocketManager* sockets,
                           ServerManager* servers)
  : _clientsManager(clients)
  , _socketsManager(sockets)
  , _serverManager(servers)
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
  if ((events & POLLIN) == 0) {
    return;
  }

  const int clientFd = _socketsManager->acceptClient(fdes);
  if (clientFd > 0) {
    const Server* const server = _serverManager->getInitServer(fdes);
    _clientsManager->addClient(clientFd, server);
    std::cout << "[SERVER] new client connected, fd=" << clientFd << '\n';
  } else {
    // std::cerr << "[SERVER] accepting new client failed\n";
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
    const long timeout = Config::getDefaultTimeout();
    const int timeoutMs = convertToMs(timeout);
    std::cout << "No clients - use default timeout: " << timeoutMs << "ms\n";
    return timeoutMs;
  }

  const long minRemaining = _clientsManager->getMinTimeout();
  const int timeoutMs = convertToMs(minRemaining);
  // std::cout << "using client min remaining timeout: " << timeoutMs << "ms\n";

  return timeoutMs;
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
  return ready;
}
