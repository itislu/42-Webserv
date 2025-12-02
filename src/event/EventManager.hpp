#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "client/ClientManager.hpp"
#include "socket/SocketManager.hpp"
#include <client/Client.hpp>
#include <utils/logger/Logger.hpp>

class ServerManager;

/* TODO: change this to namespace? */
class EventManager
{
public:
  static int check();
  static void checkTimeouts();

private:
  EventManager();
  static ClientManager& _clientManager();
  static ServerManager& _serverManager();
  static SocketManager& _socketManager();

  /* EVENTS */
  static void checkActivity();
  static bool handleClient(Client* client, unsigned events);
  static bool sendToClient(Client& client);
  static bool receiveFromClient(Client& client);
  static void clientStateMachine(Client& client);
  static void disconnectClient(Client* client);
  static void acceptClient(int fdes, unsigned events);

  /* TIMEOUT */
  static int calculateTimeout();

  static Logger& _log;
};

#endif
