#include "Server.hpp"
#include "client/Client.hpp"
#include <algorithm>
#include <csignal>
#include <cstddef>
#include <cstdlib> //exit()
#include <cstring> //std::memset()
#include <errno.h> //errno
#include <fcntl.h> //fcntl()
#include <iostream>
#include <netinet/in.h> //struct sockaddr
#include <new>
#include <signal.h>
#include <string.h> // strerror()
#include <string>
#include <sys/poll.h>
#include <sys/socket.h> //socket(), setsockopt(),
#include <sys/types.h>
#include <unistd.h> //close()
#include <vector>

volatile sig_atomic_t Server::_running = 0;

static void error(const std::string& msg)
{
  std::cerr << "Error: " << msg << " (" << strerror(errno) << ")\n";
}

static void sigIntHandler(int /*sigNum*/)
{
  Server::stopServer();
}

void Server::stopServer()
{
  _running = 0;
}

Server::Server(int port)
  : _port(port)
  , _serverFd(0)
{
  _pfds.reserve(MAX_CLIENTS); // should come from config for now 1024
}

void Server::initServer()
{
  // NOLINTBEGIN
  if (signal(SIGINT, sigIntHandler) == SIG_ERR) {
    error("Failed to set SIGINT handler");
    return;
  }
  // NOLINTEND

  initSocket();

  struct pollfd pfd = {};
  pfd.fd = _serverFd;
  pfd.events = POLLIN;
  pfd.revents = 0;

  _pfds.push_back(pfd);
}

Server::~Server()
{
  for (std::size_t i = 0; i < _pfds.size(); i++) {
    close(_pfds[i].fd);
  }
  _pfds.clear();
  _clients.clear();
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
    return;
  }

  if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
    close(clientFd);
    error("failed to accept new client");
    return;
  }

  struct pollfd pfd = {};
  pfd.fd = clientFd;
  pfd.events = POLLIN;
  pfd.revents = 0;
  _pfds.push_back(pfd);

  _clients.push_back(Client(clientFd));

  std::cout << "[SERVER] new client connected, fd=" << clientFd << '\n';
}

void Server::disconnectClient(Client& client, std::size_t& idx)
{
  std::cout << "[SERVER] Client " << idx << " disconnected\n";
  close(client.getFd());
  _pfds.erase(_pfds.begin() + static_cast<long>(idx));
  _clients.erase(_clients.begin() + static_cast<long>(idx - 1));
  idx--;
}

void Server::receiveFromClient(Client& client, std::size_t& idx)
{
  Buffer buffer(MAX_CHUNK);
  const ssize_t bytes = recv(client.getFd(), &buffer[0], buffer.size(), 0);
  if (bytes > 0) {
    try {
      client.addToInBuff(buffer);
    } catch (std::bad_alloc& e) {
      error("Allocation for inbuffer failed");
      disconnectClient(client, idx);
    }

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
  const std::size_t maxChunk = MAX_CHUNK;
  const std::size_t toSend = std::min(client.getOutBuff().size(), maxChunk);
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

void Server::checkActivity()
{
  for (std::size_t i = 0; i < _pfds.size(); i++) {
    const unsigned events = static_cast<unsigned>(_pfds[i].revents);
    if (_pfds[i].fd == _serverFd) {
      if ((events & POLLIN) != 0) {
        acceptClient();
      }
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

void Server::run()
{
  _running = 1;
  while (_running != 0) {
    try {
      const int ready = poll((&_pfds[0]), _pfds.size(), -1);
      //-1 = no timeout
      if (ready < 0) {
        if (errno == EINTR) {
          continue;
        }
        std::cerr << "Error: poll failed\n";
        break;
      }
      if (ready == 0) {
        std::cerr << "Error: poll timeout\n";
        break;
      }

      checkActivity();

    } catch (const std::bad_alloc& e) {
      std::cerr << "Error: " << e.what() << "\n";
    }
  }
  std::cout << "Shutting down server...\n";
}
