#include "EventManager.hpp"

#include <client/Client.hpp>
#include <client/ClientManager.hpp>
#include <client/TimeStamp.hpp>
#include <config/Config.hpp>
#include <http/http.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <server/Server.hpp>
#include <server/ServerManager.hpp>
#include <socket/SocketManager.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/StateHandler.hpp>

#include <cstddef>
#include <cstring>
#include <exception>
#include <iostream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <vector>

/* **************************************************************************
 */
// INIT

Logger& EventManager::_log = Logger::getInstance(LOG_SERVER);

/* ************************************************************************** */

// Handles the main poll loop:
// Calls poll()
// Calls checkActivity() (or delegate to ClientManager/SocketManager)
// Checks timeouts

ClientManager& EventManager::_clientManager()
{
  return ClientManager::getInstance();
}

ServerManager& EventManager::_serverManager()
{
  return ServerManager::getInstance();
}

SocketManager& EventManager::_socketManager()
{
  return SocketManager::getInstance();
}

bool EventManager::sendToClient(Client& client)
{
  const bool alive = client.sendTo();
  if (alive && !client.hasDataToSend()) {
    _socketManager().disablePollout(client.getFd());
    _log.info() << "Pollout disabled\n";
  }
  return alive;
}

bool EventManager::receiveFromClient(Client& client)
{
  const bool alive = client.receive();
  if (alive && client.hasDataToSend()) {
    _socketManager().enablePollout(client.getFd());
  }
  return alive;
}

void EventManager::clientStateMachine(Client& client)
{
  StateHandler<Client>& handler = client.getStateHandler();

  handler.setStateHasChanged(true);
  while (!handler.isDone() && handler.stateHasChanged()) {
    handler.setStateHasChanged(false);
    handler.getState()->run();
  }
}

bool EventManager::handleClient(Client* client, unsigned events)
try {
  if (client == FT_NULLPTR) {
    return false;
  }
  bool alive = true;
  if (pollInEnabled(events) && alive) { // Receive Data
    alive = receiveFromClient(*client);
  }
  if (alive) {
    clientStateMachine(*client);
  }
  if (!pollOutEnabled(events) && alive && client->hasDataToSend()) {
    _socketManager().enablePollout(client->getFd());
    _log.info() << "Pollout enabled\n";
  }
  if (pollOutEnabled(events) && alive) { // Send Data
    alive = sendToClient(*client);
  }
  if ((events & static_cast<unsigned>(POLLHUP | POLLERR)) != 0 && alive) {
    return false; // disconnect client
  }
  return alive;
} catch (const std::exception& e) {
  _log.error() << *client << " EventManager: " << e.what() << '\n';
  handleException(client);
  return false;
}

void EventManager::disconnectClient(Client* client)
{
  if (client == FT_NULLPTR) {
    return;
  }
  const int clientFd = client->getFd();
  // Remove corresponding pollfd
  _socketManager().removeFd(clientFd);

  // remove from client map
  _clientManager().removeClient(clientFd);

  std::cout << "[SERVER] Client fd=" << clientFd << " disconnected\n";
}

void EventManager::acceptClient(int fdes, const unsigned events)
{
  if ((events & POLLIN) == 0) {
    return;
  }

  const int clientFd = _socketManager().acceptClient(fdes);
  if (clientFd > 0) {
    const Server* const server = _serverManager().getInitServer(fdes);
    _clientManager().addClient(clientFd, server);
    std::cout << "[SERVER] new client connected, fd=" << clientFd << '\n';
  } else {
    std::cerr << "[SERVER] accepting new client failed\n";
  }
}

void EventManager::checkActivity()
{
  const SocketManager& socketManager = _socketManager();
  const std::vector<pollfd>& pfds = socketManager.getPfds();
  for (std::size_t i = 0; i < pfds.size();) {
    const unsigned events = static_cast<unsigned>(pfds[i].revents);
    if (socketManager.isListener(pfds[i].fd)) {
      acceptClient(pfds[i].fd, events);
      i++;
    } else {
      Client* const client = _clientManager().getClient(pfds[i].fd);
      if (!handleClient(client, events)) {
        disconnectClient(client);
      } else {
        i++;
      }
    }
  }
}

int EventManager::calculateTimeout()
{
  // No clients yet, get default
  if (!_clientManager().hasClients()) {
    const long timeout = Config::getDefaultTimeout();
    const int timeoutMs = convertSecondsToMs(timeout);
    std::cout << "No clients - use default timeout: " << timeoutMs << "ms\n";
    return timeoutMs;
  }

  const long minRemaining = _clientManager().getMinTimeout();
  const int timeoutMs = convertSecondsToMs(minRemaining);
  std::cout << "using client min remaining timeout: " << timeoutMs << "ms\n";

  return timeoutMs;
}

int EventManager::check()
{
  const int timeout = calculateTimeout();
  const int ready = poll(
    _socketManager().getPfdStart(), _socketManager().getPfdsSize(), timeout);
  if (ready <= 0) {
    return ready;
  }
  try {
    checkActivity();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
  }
  return ready;
}

void EventManager::checkTimeouts()
{
  std::vector<ft::shared_ptr<Client> > timedOut;
  _clientManager().getTimedOutClients(timedOut);
  for (std::size_t i = 0; i < timedOut.size(); ++i) {
    std::cout << "[SERVER] Client fd=" << timedOut[i]->getFd()
              << " timed out.\n";
    disconnectClient(timedOut[i].get());
  }
}

bool EventManager::pollInEnabled(unsigned events)
{
  return (events & POLLIN) != 0;
}

bool EventManager::pollOutEnabled(unsigned events)
{
  return (events & POLLOUT) != 0;
}

void EventManager::handleException(Client* client)
{
  /**  //todo
   * should send response message only if nothing has been sent yet.
   * tricky for piped requests
   */
  const char* const msg = http::minResponse500;
  const std::size_t msgLen = std::strlen(msg);
  (void)send(client->getFd(), msg, msgLen, 0);
}
