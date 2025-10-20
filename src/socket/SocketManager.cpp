#include "SocketManager.hpp"
#include "config/Config.hpp"
#include "Socket.hpp"


// Owns _portToSocket and _pfds.
// Handles creating listeners and pollfds.

// Can expose:
// addSocket(Socket*)
// isListener(int fd)
// getSocketFromFd(int fd)
// getPollFdForClient(Client*)

SocketManager::SocketManager(const Config& config)
{
	createListeningSockets(config.getServers());
}

void SocketManager::createListeningSockets(const std::vector<ServerConfig>& configs)
{
	for (std::vector<ServerConfig>::const_iterator it = configs.begin(); it != configs.end(); ++it)
	{
		createListener((*it).getPorts());
	}
}

void SocketManager::createListener(const std::vector<int>ports)
{
	for (std::vector<int>::const_iterator it = ports.begin(); it != ports.end(); ++it)
	{
		if (listenerExists(*it))
			continue;
		Socket* socket = new Socket(*it);
		addToFdSocketMap(socket->getFd(), socket);   
		addToPfd(socket->getFd());
	}
}

bool SocketManager::listenerExists(int port) const
{
	for (std::map<int, const Socket*>::const_iterator it = _fdToSocket.begin(); it != _fdToSocket.end(); ++it)
	{
		if ((*it).second->getPort() == port)
		{
			return true;
		}
	}
	return false;
}

void SocketManager::addToFdSocketMap(int fd, const Socket* socket)
{
	_fdToSocket.insert(std::make_pair(fd,socket));
}

const Socket* SocketManager::getSocket(int fd) const
{
	std::map<int, const Socket*>::const_iterator it = _fdToSocket.find(fd);
	if (it != _fdToSocket.end())
		return it->second;
	return NULL;
}

void SocketManager::addToPfd(int fd)
{
  struct pollfd pfd = {};
  pfd.fd = fd;
  pfd.events = POLLIN;
  pfd.revents = 0;
  _pfds.push_back(pfd);
}

const Socket* SocketManager::getListener(int port) const
{
	for (std::map<int, const Socket*>::const_iterator it = _fdToSocket.begin(); it != _fdToSocket.end(); ++it)
	{
		if ((*it).second->getPort() == port)
		{
			return it->second;
		}
	}
	return NULL;
}
