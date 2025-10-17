#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include "Client.hpp"
#include "server/ServerManager.hpp"
#include "socket/SocketManager.hpp"
#include <vector>

class ClientManager
{
public:
private:
  std::vector<Client*> _clients;
  SocketManager& _sockets;
  ServerManager& _servers;
};
#endif
