#include "TimeoutManager.hpp"

// Optional, but useful if you have different timeouts per client/server.
// Could provide:
// long getRemainingTime(Client*)
// int calculatePollTimeout(const std::vector<Client*>& clients)