#include "Server.hpp"
#include "client/Client.hpp"
#include "config/Config.hpp"
#include "socket/Socket.hpp"
#include "utils/Buffer.hpp"
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

Server::~Server()
{
  for (std::size_t i = 0; i < _pfds.size(); i++) {
    close(_pfds[i].fd);
  }
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
    it->initSocket();
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

void Server::acceptClient(int serverFd)
{
  const int clientFd = accept(serverFd, NULL, NULL);
  if (clientFd < 0) {
    error("failed to accept new client");
    return;
  }

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg): POSIX C API.
  if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
    close(clientFd);
    error("failed to set client socket to non-blocking");
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
  _clients.erase(_clients.begin() + static_cast<long>(idx - _listeners.size()));
  return false;
}

bool Server::receiveFromClient(Client& client, std::size_t& idx)
{
  Buffer::Container buffer(MAX_CHUNK);
  const ssize_t bytes = recv(client.getFd(), &buffer[0], buffer.size(), 0);
  if (bytes > 0) {
    client.getInBuff().add(buffer);
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

bool Server::isListener(int sockFd)
{
  for (std::vector<Socket>::iterator it = _listeners.begin();
       it != _listeners.end();
       ++it) {
    if (sockFd == it->getFd()) {
      return true;
    }
  }
  return false;
}

bool Server::sendToClient(Client& client, std::size_t& idx)
{
  const std::size_t maxChunk = MAX_CHUNK;
  const std::size_t toSend = std::min(client.getOutBuff().getSize(), maxChunk);
  const ssize_t bytes =
    send(client.getFd(), client.getOutBuff().data(), toSend, 0);
  if (bytes > 0) {
    client.getOutBuff().remove(bytes);
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
    if (isListener(_pfds[i].fd)) {
      if ((events & POLLIN) != 0) {
        acceptClient(_pfds[i].fd);
      }
    } else {
      Client& client = _clients[i - _listeners.size()];
      try {
        if ((events & POLLIN) != 0 && same) { // Receive Data
          same = receiveFromClient(client, i);
        }
        if ((events & POLLOUT) != 0 && same) { // Send Data
          same = sendToClient(client, i);
        }
      } catch (const std::bad_alloc& e) {
        std::cerr << "Error: " << e.what() << "\n";
        disconnectClient(client, i);
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
  }
  std::cout << "Shutting down server...\n";
}
