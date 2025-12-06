#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include "Client.hpp"
#include "libftpp/memory.hpp"
#include "server/Server.hpp"
#include "socket/Socket.hpp"
#include <cstddef>
#include <map>
#include <vector>

class ClientManager
{
public:
  typedef std::map<int, ft::shared_ptr<Client> >::iterator FdToClientIter;
  typedef std::map<int, ft::shared_ptr<Client> >::const_iterator
    const_FdToClientIter;

  static ClientManager& getInstance();
  ~ClientManager() {}

  Client* getClient(int fdes) const;
  std::size_t getClientCount() const;
  const std::map<int, ft::shared_ptr<Client> >& getClients() const;
  bool hasClients() const;

  void addClient(int fdes, const Server* server, const Socket& socket);
  void removeClient(int fdes);

  long getMinTimeout() const;
  void getTimedOutClients(std::vector<ft::shared_ptr<Client> >& timedOut) const;

private:
  ClientManager() {}
  ClientManager(const ClientManager& other);
  ClientManager& operator=(const ClientManager& other);

  std::map<int, ft::shared_ptr<Client> > _clients;
};

#endif
