#include "Server.hpp"

void error(const std::string& msg)
{
  std::cerr << "Error: " << msg << " (" << strerror(errno) << ")\n";
  exit(1);
}

Server::Server(int port) : _port(port)
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

  struct sockaddr_in serverAddr;
  std::memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //htonl(2130706433); localhost: 127.0.0.1
  serverAddr.sin_port = htons(_port);

  if (bind(_serverFd, (const struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
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
  if (clientFd < 0)
    error("failed to accept new client");

  fcntl(clientFd, F_SETFL, O_NONBLOCK);

  Client client(clientFd);
  _clients.push_back(client);

  struct pollfd pfd;
  pfd.fd = clientFd;
  pfd.events = POLLIN;
  pfd.revents = 0;
  _pfds.push_back(pfd);

  std::cout << "[SERVER] new client connected, fd=" << clientFd << std::endl;
}

void Server::disconnectClient(Client& client, size_t i)
{
  close(client.getFd());
  _pfds.erase(_pfds.begin() + i);
  _clients.erase(_clients.begin() + i - 1);
}

void Server::handleClient(Client& client, size_t i)
{
  //CLIENT STATE READING
  char buffer[1024];
  int bytes = recv(client.getFd(), buffer, sizeof(buffer), 0);
  if (bytes > 0)
  {
    client.addToInBuff(buffer, bytes);
    buffer[bytes] = '\0';
    std::cout << "Client " << i << ": " << buffer;
    //TODO: STATEMACHINE
    //processBuffer(client);
  }
  else if (bytes == 0)
  {
    std::cout << "[SERVER] Client " << i << " disconnected\n";
    disconnectClient(client, i);
  }
  else //bytes < 0
  {
    std::cout << "[SERVER] No data from Client " << i << std::endl;
  }

  //CLIENT STATE SENDING...
    //send(client.getFd(), response, response.size(), 0);
    //send(fd, buffer, sizeof(buffer), flags);
}

void Server::run()
{
  bool run = true;
  while (run)
  {
    int ready = poll((&_pfds[0]), _pfds.size() , -1); //-1 keeps waiting without timeout
    if (ready < 0)
      error("poll failed");
    for (size_t i  = 0; i < _pfds.size(); i++)
    {
      if (_pfds[i].revents & POLLIN)
      {
        if(_pfds[i].fd == _serverFd)
          acceptClient();
        else
          handleClient(_clients[i - 1], i);
      }
    }
  }
}
