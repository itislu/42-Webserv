#include "SocketManager.hpp"

// Owns _portToSocket and _pfds.
// Handles creating listeners and pollfds.

// Can expose:
// addSocket(Socket*)
// isListener(int fd)
// getSocketFromFd(int fd)
// getPollFdForClient(Client*)

// Handles just poll-related bookkeeping.