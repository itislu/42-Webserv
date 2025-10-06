#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <sys/types.h>
#include <vector>

class Client
{
public:
  typedef std::vector<unsigned char> Buffer;
  Client();
  explicit Client(int sockFd);

  int getFd() const;
  Buffer getInBuff() const;
  Buffer getOutBuff() const;

  void addToInBuff(std::string str);
  void addToInBuff(const Buffer& buffer);

  void addToOutBuff(std::string str);
  void addToOutBuff(const Buffer& buffer);

  void removeFromOutBuff(ssize_t bytes);

  bool hasDataToSend() const;

private:
  int _fd;
  int _state;
  Buffer _inBuff;
  Buffer _outBuff;
};

#endif
