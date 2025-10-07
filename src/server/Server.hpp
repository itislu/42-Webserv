#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstddef>
#include <string>
#include <sys/poll.h> //poll(), struct pollfd
#include <vector>

#include "client/Client.hpp"
#include "config/Config.hpp"
#include "socket/Socket.hpp"

#ifndef MAX_CHUNK
#define MAX_CHUNK 1024
#endif
class Server
{
public:
  typedef std::vector<unsigned char> Buffer;
  explicit Server(const Config& config);
  ~Server();

  void run();
  void initServer();
  void initListeners();
  void addToPfd(int sockFd);
  void addListeners();
  bool isListener(int sockFd);
  void acceptClient(int serverFd);
  bool receiveFromClient(Client& client, std::size_t& idx);
  bool disconnectClient(Client& client, std::size_t& idx);
  bool sendToClient(Client& client, std::size_t& idx);
  void checkActivity();

  void throwSocketException(const std::string& msg);

private:
  Server(const Server& other);
  Server& operator=(const Server& other);
  std::vector<Socket> _listeners;
  std::vector<Client> _clients;
  std::vector<pollfd> _pfds;
};

#endif
