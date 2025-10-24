#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include "Client.hpp"
#include "server/Server.hpp"
#include <cstddef>
#include <map>
#include <vector>

class ClientManager
{
public:
  typedef std::map<int, Client*>::iterator clientIter;
  typedef std::map<int, Client*>::const_iterator const_clientIter;

  ClientManager();
  ~ClientManager();

  Client* getClient(int fdes) const;
  std::size_t getClientCount() const;
  const std::map<int, Client*>& getClients() const;
  bool hasClients() const;

  void addClient(int fdes, const Server* server);
  void removeClient(int fdes);

  long getMinTimeout() const;
  void getTimedOutClients(std::vector<Client*>& timedOut) const;

private:
  ClientManager(const ClientManager& other);
  ClientManager& operator=(const ClientManager& other);
  std::map<int /* fd */, Client*> _clients;
};

#endif
