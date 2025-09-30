#include "Server.hpp"
#include <cstdlib> //exit()
#include <cstring> //std::memset()
#include <errno.h> //errno
#include <fcntl.h> //fcntl()
#include <iostream>
#include <netinet/in.h> //struct sockadrr
#include <string.h>     // strerror()
#include <sys/socket.h> //socket(), setsockopt(),
#include <unistd.h>     //close()

void error(const std::string& msg)
{
  std::cerr << "Error: " << msg << " (" << strerror(errno) << ")\n";
  exit(1);
}

Server::Server(int port)
  : _port(port)
{
  initSocket();

  struct pollfd pfd;
  pfd.fd = _serverFd;
  pfd.events = POLLIN;
  pfd.revents = 0;

  _pfds.push_back(pfd);
}

Server::~Server()
{
  close(_serverFd);
}

void Server::initSocket()
{
  _serverFd = socket(AF_INET, SOCK_STREAM, 0);
  if (_serverFd < 0)
    error("server socket creation failed");

  int opt = 1;
  if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    error("setting options for server socket failed");

  struct sockaddr_in servAddr;
  std::memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(_port);

  if (bind(_serverFd, (const struct sockaddr*)&servAddr, sizeof(servAddr)) < 0)
    error("failed to bind server socket");

  if (listen(_serverFd, SOMAXCONN) < 0)
    error("failed to set server socket to listen");

  if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) < 0)
    error("failed to set server socket to non blocking");

  std::cout << "[SERVER] listening on port " << _port << "\n";
}

void Server::acceptClient()
{
  int clientFd = accept(_serverFd, NULL, NULL);
  if (clientFd < 0) {
    error("failed to accept new client");
  }

  if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
    close(clientFd);
    error("failed to set client non-blocking");
  }

  struct pollfd pfd;
  pfd.fd = clientFd;
  pfd.events = POLLIN;
  pfd.revents = 0;
  _pfds.push_back(pfd);

  Client client(clientFd);
  _clients.push_back(client);

  std::cout << "[SERVER] new client connected, fd=" << clientFd << std::endl;
}

void Server::disconnectClient(Client& client, size_t& i)
{
  std::cout << "[SERVER] Client " << i << " disconnected\n";
  close(client.getFd());
  _pfds.erase(_pfds.begin() + i);
  _clients.erase(_clients.begin() + i - 1);
  i--;
}

void Server::receiveFromClient(Client& client, size_t& i)
{
  char buffer[1024];
  int bytes = recv(client.getFd(), buffer, sizeof(buffer), 0);
  if (bytes > 0) {
    client.addToInBuff(buffer, bytes);
    std::cout << "Client " << i << ": ";
    std::cout.write(buffer, bytes);

    // TODO: STATEMACHINE/PARSING

    if (client.hasDataToSend())
      _pfds[i].events |= POLLOUT; // enable POLLOUT
  } else if (bytes == 0) {
    disconnectClient(client, i);
  } else // bytes < 0
  {
    std::cout << "[SERVER] No data from Client " << i << std::endl;
  }
}

void Server::sendToClient(Client& client, size_t i)
{
  size_t maxChunk = 1024;
  size_t toSend = std::min(client.getOutBuff().size(), maxChunk);
  int bytes = send(client.getFd(), client.getOutBuff().data(), toSend, 0);
  if (bytes > 0) {
    client.removeFromOutBuff(bytes);
    if (!client.hasDataToSend()) {
      _pfds[i].events &= ~POLLOUT; // disable POLLOUT
    }
  }
}

void Server::run()
{
  bool run = true;
  while (run) {
    int ready = poll((&_pfds[0]), _pfds.size(), -1); //-1 = no timeout
    if (ready < 0)
      error("poll failed");
    for (size_t i = 0; i < _pfds.size(); i++) {
      if (_pfds[i].fd == _serverFd && (_pfds[i].revents & POLLIN))
        acceptClient();
      else {
        Client& client = _clients[i - 1];
        if (_pfds[i].revents & POLLIN) // Receive Data
          receiveFromClient(client, i);
        if ((_pfds[i].revents & POLLOUT)) // Send Data
          sendToClient(client, i);
        if (_pfds[i].revents & (POLLHUP | POLLERR)) // Error
          disconnectClient(client, i);
      }
    }
  }
}
