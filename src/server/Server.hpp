#ifndef SERVER_HPP
#define SERVER_HPP

#include <poll.h> //poll(), struct pollfd
#include <vector>

#include "client/Client.hpp"

class Server
{
public:
  Server(int port);
  ~Server();

  void run();
  void initSocket();
  void acceptClient();
  void receiveFromClient(Client& client, size_t i);
  void disconnectClient(Client& client, size_t i);
  void sendToClient(Client& client, size_t i);

private:
  int _port;
  int _serverFd;
  std::vector<Client> _clients;
  std::vector<pollfd> _pfds;
};

#endif
