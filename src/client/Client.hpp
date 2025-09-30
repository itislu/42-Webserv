#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <poll.h>
#include <string>
#include <vector>

class Client
{
public:
  typedef std::vector<unsigned char> Buffer;
  Client();
  Client(int fd, struct pollfd* pfd);
  ~Client();

  int getFd() const;
  Buffer getInBuff() const;
  Buffer getOutBuff() const;

  void addToInBuff(std::string str);
  void addToInBuff(char* buffer, int bytes);

  void addToOutBuff(std::string str);
  void addToOutBuff(char* buffer, int bytes);

  void removeFromOutBuff(int bytes);

  bool hasDataToSend() const;

private:
  int _fd;
  int _state;
  Buffer _inBuff;
  Buffer _outBuff;
  struct pollfd* _pfd;
};

#endif
