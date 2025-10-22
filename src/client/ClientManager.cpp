#include "ClientManager.hpp"
#include "client/Client.hpp"
#include "client/TimeStamp.hpp"
#include "server/Server.hpp"
#include <algorithm>
#include <climits>
#include <cstddef>
#include <map>
#include <utility>
#include <vector>

ClientManager::ClientManager() {}

// ADD destructor for freeing clients

Client* ClientManager::getClient(int fdes) const
{
  const const_clientIter iter = _clients.find(fdes);
  if (iter == _clients.end()) {
    return NULL;
  }
  return iter->second;
}

std::size_t ClientManager::getClientCount() const
{
  return _clients.size();
}

// TODO: protection - allocation could fail
void ClientManager::addClient(int fdes, const Server* server)
{
  _clients.insert(std::make_pair(fdes, new Client(fdes, server)));
}

void ClientManager::removeClient(int fdes)
{
  const clientIter iter = _clients.find(fdes);
  if (iter != _clients.end()) {
    delete iter->second;
    _clients.erase(iter);
  }
}

const std::map<int, Client*>& ClientManager::getClients() const
{
  return _clients;
}

bool ClientManager::hasClients() const
{
  return !_clients.empty();
}

long ClientManager::getMinTimeout() const
{
  const TimeStamp now;
  long minRemaining = LONG_MAX;
  for (const_clientIter it = _clients.begin(); it != _clients.end(); ++it) {
    const long clientTimeout = it->second->getTimeout();
    const long remaining =
      clientTimeout - (now - it->second->getLastActivity());
    minRemaining = std::min(remaining, minRemaining);
  }
  return minRemaining;
}

void ClientManager::getTimedOutClients(std::vector<Client*>& timedOut) const
{
  const TimeStamp now;
  for (const_clientIter it = _clients.begin(); it != _clients.end(); ++it) {
    const long timeout = it->second->getTimeout();
    if (now - it->second->getLastActivity() >= timeout) {
      timedOut.push_back(it->second);
    }
  }
}
