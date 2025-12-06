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
  static SocketManager& getInstance();
  ~SocketManager() {}

  bool isListener(int fdes) const;

  std::size_t getPfdsSize() const;
  const std::vector<pollfd>& getPfds() const;
  pollfd* getPfdStart();

  int acceptClient(int fdes);

  void enablePollout(int fdes);
  void disablePollout(int fdes);

  const Socket& getSocket(int fdes) const;
  const Socket& getListener(int port) const;
  const Socket& getSocketFromClientFd(int fdes) const;

  void removeFd(int fdes);

private:
  typedef std::vector<ft::shared_ptr<const Socket> >::iterator SockIter;
  typedef std::map<int, const Socket*>::iterator FdToSockIter;
  typedef std::map<int, const Socket*>::const_iterator const_FdToSockIter;

  void createListeningSockets(const std::vector<ServerConfig>& configs);
  void createListener(const std::vector<int>& ports);
  bool listenerExists(int port) const;

  void addToPfd(int fdes);
  pollfd* getPollFd(int fdes);

  void removePfd(int fdes);
  void removeFdFromMap(int fdes);

  explicit SocketManager(const Config& config);
  SocketManager(const SocketManager& other);
  SocketManager& operator=(const SocketManager& other);

  std::vector<pollfd> _pfds;
  std::vector<ft::shared_ptr<const Socket> > _sockets;
  std::map<int /* fd */, const Socket*> _listeners;
  std::map<int /* fd */, const Socket*> _fdToSocket; // clientfds to socket
};

#endif
