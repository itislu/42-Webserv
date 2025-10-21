#include "ServerHandler.hpp"
#include "client/Client.hpp"
#include "config/Config.hpp"
#include "config/ServerConfig.hpp"
#include "server/Server.hpp"
#include "socket/Socket.hpp"
#include "utils/Buffer.hpp"
#include "utils/TimeStamp.hpp"
#include <algorithm>
#include <cerrno>
#include <climits>
#include <csignal>
#include <cstddef>
#include <cstring>
#include <exception>
#include <iostream>
#include <map>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <utility>
#include <vector>

void ServerHandler::addToClients(int sockFd, int clientFd)
{
  const Socket* const sock = getSocketFromFd(sockFd);
  _clients.push_back(new Client(clientFd, sock, getServerFromSocket(sock)));
}

const Server* ServerHandler::getServerFromSocket(const Socket* socket)
{
  const std::map<const Socket*, std::vector<Server*> >::const_iterator iter =
    _socketToServers.find(socket);
  if (iter == _socketToServers.end()) {
    std::cout << "No server found for this socket\n";
    return NULL;
  }

  const std::vector<Server*>& servers = iter->second;
  if (servers.size() != 1) {
    std::cout << "Found multiple servers, don't assign any: " << servers.size()
              << "\n";
    return NULL;
  }

  std::cout << "Found only one server, assign it instantly to client\n";
  return servers[0];
}
