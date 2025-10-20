#ifndef SOCKETMANAGER_HPP
#define SOCKETMANAGER_HPP

#include <sys/poll.h>

class SocketManager
{
public:
explicit SocketManager(const Config& config);
void createListeningSockets(const std::vector<ServerConfig>& configs);
void createListener(const std::vector<int>ports);
bool listenerExists(int port) const;

void addToFdSocketMap(int fd, const Socket* socket);
void addToPfd(int fd);

const Socket* getSocket(int fd) const;
const Socket* getListener(int port) const;

private:
std::map<int, const Socket*> _fdToSocket;
std::vector<pollfd> _pfds;
};

#endif
