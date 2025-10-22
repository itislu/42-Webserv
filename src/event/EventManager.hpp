#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "client/Client.hpp"
#include "client/ClientManager.hpp"
#include "socket/SocketManager.hpp"



class ServerManager;

class EventManager
{
public:
  EventManager(ClientManager* clients,
               SocketManager* sockets,
               ServerManager* servers);

  int check();

  /* EVENTS */
  void checkActivity();
  bool handleClient(Client* client, unsigned events);
  bool sendToClient(Client* client);
  bool receiveFromClient(Client* client);
  void disconnectClient(Client* client);
  void acceptClient(int fdes, unsigned events);

  /* TIMEOUT */
  void checkTimeouts();
  int calculateTimeout() const;
  long getClientTimeout(const Client* client) const;

private:
  ClientManager* _clientsManager;
  SocketManager* _socketsManager;
  ServerManager* _serverManager;
};

#endif
