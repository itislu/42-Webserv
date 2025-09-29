#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h> //socket(), setsockopt(),
#include <netinet/in.h> //struct sockadrr
#include <fcntl.h> //fcntl()
#include <string.h> // strerror()
#include <poll.h> //poll(), struct pollfd
#include <cstring> //std::memset()
#include <unistd.h> //close()
#include <vector>

#include "Client.hpp"

class Server
{
  public:
    Server(int port);
    ~Server();

    void run();
    void initSocket();
    void acceptClient();
    void handleClient(Client& client, size_t i);
    void disconnectClient(Client& client, size_t i);

  private:
    int _port;
    int _serverFd;
    std::vector<Client> _clients;
    std::vector<pollfd> _pfds;
};

#endif