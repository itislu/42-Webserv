#ifndef SERVER_HPP
#define SERVER_HPP

#include <bits/types.h>
#include <cstddef>
#include <sys/poll.h> //poll(), struct pollfd
#include <vector>

#include "client/Client.hpp"

#ifndef MAX_CHUNK
#define MAX_CHUNK 1024
#endif
class Server
{
public:
  typedef std::vector<unsigned char> Buffer;
  explicit Server(int port);
  Server(const Server& other);
  Server& operator=(const Server& other);
  ~Server();

  void run();
  void initSocket();
  void acceptClient();
  void receiveFromClient(Client& client, std::size_t& idx);
  void disconnectClient(Client& client, std::size_t& idx);

  static void stopServer();
  static void sendToClient(Client& client, pollfd& pfd);

private:
  int _port;
  int _serverFd;
  std::vector<Client> _clients;
  std::vector<pollfd> _pfds;
  static volatile __sig_atomic_t _running;
};

#endif
