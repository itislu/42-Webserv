#ifndef SOCKETMANAGER_HPP
#define SOCKETMANAGER_HPP

#include "config/Config.hpp"
#include "config/ServerConfig.hpp"
#include "socket/Socket.hpp"
#include <cstddef>
#include <map>
#include <sys/poll.h>
#include <vector>

class SocketManager
{
public:
  typedef std::vector<const Socket*>::iterator sockIter;
  typedef std::map<int, const Socket*>::iterator fdToSockIter;
  typedef std::map<int, const Socket*>::const_iterator const_fdToSockIter;

  explicit SocketManager(const Config* config);
  ~SocketManager();

  bool isListener(int fdes) const;

  std::size_t getPfdsSize() const;
  const std::vector<pollfd>& getPfds() const;
  pollfd* getPfdStart();

  int acceptClient(int fdes);

  void enablePollout(int fdes);
  void disablePollout(int fdes);

  const Socket& getSocket(int fdes) const;
  const Socket& getListener(int port) const;

  void removeFd(int fdes);

private:
  void createListeningSockets(const std::vector<ServerConfig>& configs);
  void createListener(const std::vector<int>& ports);
  bool listenerExists(int port) const;

  void addToPfd(int fdes);
  pollfd* getPollFd(int fdes);

  void removePfd(int fdes);
  void removeFdFromMap(int fdes);

  SocketManager(const SocketManager& other);
  SocketManager& operator=(const SocketManager& other);

  std::vector<pollfd> _pfds;
  std::vector<const Socket*> _sockets;
  std::map<int /* fd */, const Socket*> _listeners;
  std::map<int /* fd */, const Socket*> _fdToSocket; // clientfds to socket
};

#endif
