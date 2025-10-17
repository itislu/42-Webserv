#ifndef SERVERHANDLER_HPP
#define SERVERHANDLER_HPP

#include "Server.hpp"
#include "client/Client.hpp"
#include "config/Config.hpp"
#include "config/ServerConfig.hpp"
#include "socket/Socket.hpp"
#include <cstddef>
#include <map>
#include <sys/poll.h>
#include <vector>

#ifndef MAX_CHUNK
#define MAX_CHUNK 1024
#endif

class ServerHandler
{
public:
  typedef std::map<int, const Socket*>::const_iterator portToSocketIter;
  typedef std::map<const Socket*, std::vector<Server*> >::const_iterator
    socketToServersIter;
  typedef std::vector<Server*>::const_iterator serverIter;
  typedef std::vector<Client*>::const_iterator clientIter;
  typedef std::vector<pollfd>::iterator pfdIter;

  // CONSTRUCTORS
  explicit ServerHandler(const Config& config);
  ~ServerHandler();

  // RUN - Event Loop
  void run();
  void checkActivity();
  bool isListener(int sockFd);
  void acceptClient(int sockFd);
  bool handleClient(Client* client, unsigned events);
  bool receiveFromClient(Client* client);
  bool sendToClient(Client* client);
  void disconnectClient(Client* client);

  Client* getClientFromFd(int clientFd);
  pollfd* getPollFdForClient(Client* client);
  const Socket* getSocketFromFd(int sockFd);
  const Server* getServerFromSocket(const Socket* socket);

  int calculateTimeOut() const;

  // INIT - Server Setup
  void createServers(const Config& config);
  void initListeners();
  void addToPfd(int fdes);
  void addToClients(int sockFd, int clientFd);
  std::vector<const Socket*> createListeners(const std::vector<int>& ports);
  void mapServerToListeners(const std::vector<int>& ports, Server* server);
  const Socket* getListener(int port);
  void mapListeners();
  void addServerToListener();
  void checkClientTimeouts();

  // DEBUG
  void debugPrintMaps() const;

private:
  ServerHandler(const ServerHandler& other);
  ServerHandler& operator=(const ServerHandler& other);

  long _lowestTimeOut;
  const Config* _config;

  std::vector<Client*> _clients;
  std::vector<Server*> _servers;
  std::vector<pollfd> _pfds;

  std::map<const Socket*, std::vector<Server*> > _socketToServers;
  std::map<int, const Socket*> _portToSocket;
};

#endif
