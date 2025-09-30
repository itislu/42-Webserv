#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <unistd.h>
#include <vector>

class Client
{
public:
  typedef std::vector<unsigned char> Buffer;
  Client(int fd);
  ~Client();

  int getFd() const;
  Buffer getInBuff() const;

  void addToInBuff(std::string str);
  void addToInBuff(char* buffer, int bytes);

private:
  int _fd;
  int _state;
  Buffer inBuff;
  Buffer outBuff;
};

#endif
