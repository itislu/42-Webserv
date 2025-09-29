#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <unistd.h>
#include <iostream>
#include <vector>

class Client
{
  public:
    typedef std::vector<unsigned char> Buffer;

    Client(int fd);
    ~Client();

    int getFd() const;
    int getBytes() const;

    void setBytes(int bytes);
  
    void addToInBuff(char* buffer, int bytes);

  private:
    int _fd;
    int _state;
    int _bytes;
    Buffer inBuff;
    Buffer outBuff;
};

#endif