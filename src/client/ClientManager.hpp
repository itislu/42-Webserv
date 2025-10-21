#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include "Client.hpp"
#include <cstddef>
#include <map>
#include <vector>

class ClientManager
{
public:
  typedef std::map<int, Client*>::iterator clientIter;
  typedef std::map<int, Client*>::const_iterator const_clientIter;

  ClientManager();

  Client* getClient(int fdes) const;
  std::size_t getClientCount() const;

  bool hasClients() const;
  long getMinTimeout() const;
  void getTimedOutClients(  std::vector<Client*>& timedOut) const;

  const std::map<int, Client*>& getClients() const;

  void addClient(int fdes);
  void removeClient(int fdes);

private:
  std::map<int /* fd */, Client*> _clients;
};

#endif
