#include "EventManager.hpp"

// Handles the main poll loop:
// Calls poll()
// Calls checkActivity() (or delegate to ClientManager/SocketManager)
// Checks timeouts