#include "ClientManager.hpp"
#include "client/Client.hpp"
#include "client/TimeStamp.hpp"
#include "libftpp/memory.hpp"
#include "libftpp/utility.hpp"
#include "server/Server.hpp"
#include <algorithm>
#include <climits>
#include <cstddef>
#include <map>
#include <utility>
#include <vector>

Client* ClientManager::getClient(int fdes) const
{
  const const_FdToClientIter iter = _clients.find(fdes);
  if (iter == _clients.end()) {
    return FT_NULLPTR;
  }
  return iter->second.get();
}

std::size_t ClientManager::getClientCount() const
{
  return _clients.size();
}

void ClientManager::addClient(int fdes, const Server* server)
{
  _clients.insert(std::make_pair(fdes, ft::make_shared<Client>(fdes, server)));
}

void ClientManager::removeClient(int fdes)
{
  const FdToClientIter iter = _clients.find(fdes);
  if (iter != _clients.end()) {
    _clients.erase(iter);
  }
}

const std::map<int, ft::shared_ptr<Client> >& ClientManager::getClients() const
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
  for (const_FdToClientIter it = _clients.begin(); it != _clients.end(); ++it) {
    const long clientTimeout = it->second->getTimeout();
    const long remaining =
      clientTimeout - (now - it->second->getLastActivity());
    minRemaining = std::min(remaining, minRemaining);
  }
  return minRemaining;
}

void ClientManager::getTimedOutClients(
  std::vector<ft::shared_ptr<Client> >& timedOut) const
{
  const TimeStamp now;
  for (const_FdToClientIter it = _clients.begin(); it != _clients.end(); ++it) {
    const long timeout = static_cast<long>(it->second->getTimeout());
    if (now - it->second->getLastActivity() >= timeout) {
      timedOut.push_back(it->second);
    }
  }
}
