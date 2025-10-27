#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include "Client.hpp"
#include "libftpp/memory.hpp"
#include "server/Server.hpp"
#include <cstddef>
#include <map>
#include <vector>

class ClientManager
{
public:
  typedef std::map<int /* fd */, ft::shared_ptr<Client> > FdToClient;
  typedef FdToClient::iterator FdToClientIter;
  typedef FdToClient::const_iterator const_FdToClientIter;

  ClientManager() {}
  ~ClientManager() {}

  ft::shared_ptr<Client> getClient(int fdes) const;
  std::size_t getClientCount() const;
  const FdToClient& getClients() const;
  bool hasClients() const;

  void addClient(int fdes, const ft::shared_ptr<const Server>& server);
  void removeClient(int fdes);

  long getMinTimeout() const;
  void getTimedOutClients(std::vector<ft::shared_ptr<Client> >& timedOut) const;

private:
  ClientManager(const ClientManager& other);
  ClientManager& operator=(const ClientManager& other);

  FdToClient _clients;
};

#endif
