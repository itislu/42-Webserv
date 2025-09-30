#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "http/Request.hpp"
#include <string>
#include <vector>

class Client
{
public:
  typedef std::vector<unsigned char> Buffer;

  Client();
  Client(int fd);
  ~Client();

  int getFd() const;
  Buffer& getBuffer();
  void inBuffAddStr(std::string& str);
  Request& getRequest();

private:
  int _fd;

  Buffer _inBuff;
  Buffer _outBuff;

  Request _request;
};

#endif
