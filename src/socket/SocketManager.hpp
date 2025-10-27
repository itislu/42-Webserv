#ifndef SOCKETMANAGER_HPP
#define SOCKETMANAGER_HPP

#include "config/Config.hpp"
#include "config/ServerConfig.hpp"
#include "libftpp/memory.hpp"
#include "socket/Socket.hpp"
#include <cstddef>
#include <map>
#include <sys/poll.h>
#include <vector>

class SocketManager
{
public:
  explicit SocketManager(const Config& config);
  ~SocketManager() {}

  bool isListener(int fdes) const;

  std::size_t getPfdsSize() const;
  const std::vector<pollfd>& getPfds() const;
  pollfd* getPfdStart();

  int acceptClient(int fdes);

  void enablePollout(int fdes);
  void disablePollout(int fdes);

  ft::shared_ptr<const Socket> getSocket(int fdes) const;
  ft::shared_ptr<const Socket> getListener(int port) const;

  void removeFd(int fdes);

private:
  typedef std::vector<ft::shared_ptr<const Socket> > Sockets;
  typedef Sockets::iterator sockIter;
  typedef std::map<int /* fd */, ft::shared_ptr<const Socket> > FdToSock;
  typedef FdToSock::iterator fdToSockIter;
  typedef FdToSock::const_iterator const_fdToSockIter;

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
  Sockets _sockets;
  FdToSock _listeners;
  FdToSock _fdToSocket; // clientfds to socket
};

#endif
