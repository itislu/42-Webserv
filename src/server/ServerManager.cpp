#include "ServerManager.hpp"

// Owns _servers and _config.
// Creates servers and maps them to sockets.
// Responsible for server-related queries like getServerFromSocket().