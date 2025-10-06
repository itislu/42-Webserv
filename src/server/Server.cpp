#include "Server.hpp"
#include "client/Client.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdlib> //exit()
#include <cstring> //std::memset()
#include <errno.h> //errno
#include <fcntl.h> //fcntl()
#include <iostream>
#include <netinet/in.h> //struct sockadrr
#include <string.h>     // strerror()
#include <string>
#include <sys/poll.h>
#include <sys/socket.h> //socket(), setsockopt(),
#include <sys/types.h>
#include <unistd.h> //close()
#include <vector>

// TODO REMOVE THIS AND THROW EXCEPTION
static void error(const std::string& msg)
{
  std::cerr << "Error: " << msg << " (" << strerror(errno) << ")\n";
  exit(1);
}

Server::Server(int port)
  : _port(port)
  , _serverFd(0)
{
  initSocket();

  struct pollfd pfd = {};
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
  if (_serverFd < 0) {
    error("server socket creation failed");
  }

  int opt = 1;
  if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    error("setting options for server socket failed");
  }

  struct sockaddr_in servAddr = {};
  std::memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(_port);

  // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
  if (bind(_serverFd,
           reinterpret_cast<const struct sockaddr*>(&servAddr),
           sizeof(servAddr)) < 0)
  // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
  {
    error("failed to bind server socket");
  }

  if (listen(_serverFd, SOMAXCONN) < 0) {
    error("failed to set server socket to listen");
  }

  if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) < 0) {
    error("failed to set server socket to non blocking");
  }

  std::cout << "[SERVER] listening on port " << _port << "\n";
}

void Server::acceptClient()
{
  const int clientFd = accept(_serverFd, NULL, NULL);
  if (clientFd < 0) {
    error("failed to accept new client");
  }

  if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
    close(clientFd);
    error("failed to set client non-blocking");
  }

  struct pollfd pfd = {};
  pfd.fd = clientFd;
  pfd.events = POLLIN;
  pfd.revents = 0;
  _pfds.push_back(pfd);

  _clients.push_back(Client(clientFd));

  std::cout << "[SERVER] new client connected, fd=" << clientFd << '\n';
}

void Server::disconnectClient(Client& client, size_t& idx)
{
  std::cout << "[SERVER] Client " << idx << " disconnected\n";
  close(client.getFd());
  _pfds.erase(_pfds.begin() + static_cast<long>(idx));
  _clients.erase(_clients.begin() + static_cast<long>(idx - 1));
  idx--;
}

void Server::receiveFromClient(Client& client, size_t& idx)
{
  Buffer buffer(MAX_CHUNK);
  const ssize_t bytes = recv(client.getFd(), &buffer[0], buffer.size(), 0);
  if (bytes > 0) {
    client.addToInBuff(buffer);
    // This is just for debugging atm
    std::cout << "Client " << idx << ": ";
    // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
    std::cout.write(reinterpret_cast<const char*>(&buffer[0]),
                    static_cast<std::streamsize>(bytes));
    // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

    // TODO: STATEMACHINE/PARSING

    if (client.hasDataToSend()) {
      _pfds[idx].events = static_cast<short>(
        static_cast<unsigned>(_pfds[idx].events) |
        static_cast<unsigned>(POLLOUT)); // enable POLLOUT safely
    }
  } else if (bytes == 0) {
    disconnectClient(client, idx);
  } else // bytes < 0
  {
    std::cout << "[SERVER] No data from Client " << idx << '\n';
  }
}

void Server::sendToClient(Client& client, pollfd& pfd)
{
  const size_t maxChunk = MAX_CHUNK;
  const size_t toSend = std::min(client.getOutBuff().size(), maxChunk);
  const ssize_t bytes =
    send(client.getFd(), client.getOutBuff().data(), toSend, 0);
  if (bytes > 0) {
    client.removeFromOutBuff(bytes);
    if (!client.hasDataToSend()) {
      pfd.events = static_cast<short>(static_cast<unsigned>(pfd.events) &
                                      ~static_cast<unsigned>(POLLOUT));
    }
  }
}

void Server::run()
{
  while (true) {
    const int ready = poll((&_pfds[0]), _pfds.size(), -1); //-1 = no timeout
    if (ready < 0) {
      error("poll failed");
    }
    for (size_t i = 0; i < _pfds.size(); i++) {
      const unsigned events = static_cast<unsigned>(_pfds[i].revents);
      if (_pfds[i].fd == _serverFd && ((events & POLLIN) != 0)) {
        acceptClient();
      } else {
        Client& client = _clients[i - 1];
        if ((events & POLLIN) != 0) { // Receive Data
          receiveFromClient(client, i);
        }
        if ((events & POLLOUT) != 0) { // Send Data
          Server::sendToClient(client, _pfds[i]);
        }
        if ((events & static_cast<unsigned>(POLLHUP | POLLERR)) != 0) { // Error
          disconnectClient(client, i);
        }
      }
    }
  }
}
