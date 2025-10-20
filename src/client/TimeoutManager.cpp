#include "TimeoutManager.hpp"

// Optional, but useful if you have different timeouts per client/server.
// Could provide:
// long getRemainingTime(Client*)
// int calculatePollTimeout(const std::vector<Client*>& clients)

long TimeoutManager::getClientTimeOut(const Client* const client) const
{
  if (client->getServer() != 0) {
    return client->getServer()->getTimeout();
  }
  return _config->getDefaultTimeout();
}

int TimeoutManager::calculateTimeOut() const
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

void TimeoutManager::checkClientTimeouts()
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