#include "EventManager.hpp"
#include "ClientManager.hpp"
#include <csignal>
#include <string>
#include <iostream>
#include <cstring>

// Handles the main poll loop:
// Calls poll()
// Calls checkActivity() (or delegate to ClientManager/SocketManager)
// Checks timeouts

EventManager::EventManager(const ClientManager& clients, const SocketManager& sockets)
: _clientsManager(clients), _socketsManager(sockets)
{

}

int EventManager::check()
{
  const int timeOut = 1000; //calculateTimeOut();
  const int ready = poll((&_pfds[0]), _pfds.size(), timeOut);
  if (ready <= 0) {
    return ready;
  }
  checkActivity();
  return 1;
}