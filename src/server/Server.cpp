#include "Server.hpp"
#include "client/Client.hpp"
#include "config/Config.hpp"
#include "socket/Socket.hpp"
#include <algorithm>
#include <csignal>
#include <cstddef>
#include <cstdlib> //exit()
#include <cstring> //std::memset()
#include <errno.h> //errno
#include <fcntl.h> //fcntl()
#include <iostream>
#include <new>
#include <signal.h>
#include <stdexcept>
#include <string.h> // strerror()
#include <string>
#include <sys/poll.h>
#include <sys/socket.h> //socket(), setsockopt(),
#include <sys/types.h>
#include <unistd.h> //close()
#include <vector>

static volatile sig_atomic_t g_running = 0;

static void error(const std::string& msg)
{
  std::cerr << "Error: " << msg << " (" << strerror(errno) << ")\n";
}

extern "C" void sigIntHandler(int /*sigNum*/)
{
  g_running = 0;
}

Server::Server(const Config& config)
{
  _listeners.reserve(config.getPorts().size());
  for (std::vector<int>::const_iterator it = config.getPorts().begin();
       it != config.getPorts().end();
       ++it) {
    _listeners.push_back(Socket(*it));
  }

  _pfds.reserve(config.getMaxClients());
  _clients.reserve(config.getMaxClients());
}

Server::Server(const Server& other)
{
  *this = other;
}

Server& Server::operator=(const Server& other)
{
  if (this != &other) {
    _port = other._port;
    _serverFd = other._serverFd;
    _pfds = other._pfds;
    _clients = other._clients;
    _listeners = other._listeners;
  }
  return *this;
}

Server::~Server()
{
  for (std::size_t i = 0; i < _pfds.size(); i++) {
    close(_pfds[i].fd);
  }
  _pfds.clear();
  _clients.clear();
  _listeners.clear();
}

void Server::addToPfd(int sockFd)
{
  struct pollfd pfd = {};
  pfd.fd = sockFd;
  pfd.events = POLLIN;
  pfd.revents = 0;
  _pfds.push_back(pfd);
}

void Server::initListeners()
{
  for (std::vector<Socket>::iterator it = _listeners.begin();
       it != _listeners.end();
       ++it) {
    // init Socket here
    addToPfd(it->getFd());
    std::cout << "[SERVER] listening on port " << it->getPort() << "\n";
  }
}

void Server::initServer()
{
  if (signal(SIGINT, sigIntHandler) == SIG_ERR) {
    error("Failed to set SIGINT handler");
    return;
  }
  initListeners();
}

// if we wanna add a MAX_CLIENT limit -> TODO add limit check
void Server::acceptClient()
{
  const int clientFd = accept(_serverFd, NULL, NULL);
  if (clientFd < 0) {
    error("failed to accept new client");
    return;
  }

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg): POSIX C API.
  if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
    close(clientFd);
    error("failed to accept new client");
    return;
  }

  addToPfd(clientFd);
  _clients.push_back(Client(clientFd));

  std::cout << "[SERVER] new client connected, fd=" << clientFd << '\n';
}

bool Server::disconnectClient(Client& client, std::size_t& idx)
{
  std::cout << "[SERVER] Client " << idx << " disconnected\n";
  close(client.getFd());
  _pfds.erase(_pfds.begin() + static_cast<long>(idx));
  _clients.erase(_clients.begin() + static_cast<long>(idx - 1));
  return false;
}

bool Server::receiveFromClient(Client& client, std::size_t& idx)
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
    return disconnectClient(client, idx);
  } else // bytes < 0
  {
    error("recv failed, removing client");
    return disconnectClient(client, idx);
  }
  return true;
}

bool Server::sendToClient(Client& client, std::size_t& idx)
{
  const std::size_t maxChunk = MAX_CHUNK;
  const std::size_t toSend = std::min(client.getOutBuff().size(), maxChunk);
  const ssize_t bytes =
    send(client.getFd(), client.getOutBuff().data(), toSend, 0);
  if (bytes > 0) {
    client.removeFromOutBuff(bytes);
    if (!client.hasDataToSend()) {
      _pfds[idx].events =
        static_cast<short>(static_cast<unsigned>(_pfds[idx].events) &
                           ~static_cast<unsigned>(POLLOUT));
    }
  } else if (bytes == 0) {
    std::cout << "[SERVER] no data sent to client " << idx << "\n";
  } else {
    std::cerr << "[SERVER] send error for client " << idx << ": "
              << strerror(errno) << "\n";
    return disconnectClient(client, idx);
  }
  return true;
}

void Server::checkActivity()
{
  for (std::size_t i = 0; i < _pfds.size();) {
    bool same = true;
    const unsigned events = static_cast<unsigned>(_pfds[i].revents);
    if (_pfds[i].fd == _serverFd) {
      if ((events & POLLIN) != 0) {
        acceptClient();
      }
    } else {
      Client& client = _clients[i - 1];
      if ((events & POLLIN) != 0 && same) { // Receive Data
        same = receiveFromClient(client, i);
      }
      if ((events & POLLOUT) != 0 && same) { // Send Data
        same = sendToClient(client, i);
      }
      if ((events & static_cast<unsigned>(POLLHUP | POLLERR)) != 0 &&
          same) { // Error
        same = disconnectClient(client, i);
      }
    }
    if (same) {
      i++;
    }
  }
}

void Server::run()
{
  g_running = 1;
  while (g_running != 0) {
    try {
      const int ready = poll((&_pfds[0]), _pfds.size(), -1);
      //-1 = no timeout
      if (ready < 0) {
        if (errno == EINTR) {
          continue;
        }
        error("poll failed");
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

void Server::throwSocketException(const std::string& msg)
{
  if (_serverFd >= 0) {
    close(_serverFd);
    _serverFd = -1;
  }
  throw std::runtime_error(msg);
}

/* void Server::initSocket()
{
  _serverFd = socket(AF_INET, SOCK_STREAM, 0);
  if (_serverFd < 0) {
    throwSocketException("server socket creation failed");
  }

  int opt = 1;
  if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    throwSocketException("setting options for server socket failed");
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
    throwSocketException("failed to bind server socket");
  }

  if (listen(_serverFd, SOMAXCONN) < 0) {
    throwSocketException("failed to set server socket to listen");
  }

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg): POSIX C API.
  if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) < 0) {
    throwSocketException("failed to set server socket to non-blocking");
  }

  std::cout << "[SERVER] listening on port " << _port << "\n";
} */