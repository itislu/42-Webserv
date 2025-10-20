#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "client/ClientManager.hpp"
#include "socket/SocketManager.hpp"

class EventManager
{
public:
EventManager(const ClientManager& clients, const SocketManager& sockets);

int check();

private:
const ClientManager& _clientsManager;
const SocketManager& _socketsManager;

};

#endif
