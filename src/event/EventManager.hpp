#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "client/Client.hpp"
#include "client/ClientManager.hpp"
#include "socket/SocketManager.hpp"
#include "utils/logger/Logger.hpp"

class ServerManager;

class EventManager
{
public:
  EventManager(ClientManager& clients,
               SocketManager& sockets,
               ServerManager& servers);

  int check();
  void checkTimeouts();

private:
  /* EVENTS */
  void checkActivity();
  bool handleClient(Client* client, unsigned events);
  bool sendToClient(Client& client);
  bool receiveFromClient(Client& client);
  void disconnectClient(Client* client);
  void acceptClient(int fdes, unsigned events);

  /* TIMEOUT */
  int calculateTimeout() const;

  static Logger& _log;
  ClientManager* _clientsManager;
  SocketManager* _socketsManager;
  ServerManager* _serverManager;
};

#endif
