#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "http/Request.hpp"
#include <utils/state/IState.hpp>
#include "utils/Buffer.hpp"
#include <string>
#include <sys/types.h>
#include <vector>

class Client
{
public:
  Client();
  explicit Client(int sockFd);

  int getFd() const;
  Buffer& getInBuff();
  Buffer& getOutBuff();
  Request& getRequest();

  void addToInBuff(const std::string& str);
  void addToInBuff(const Buffer& buffer);

  void addToOutBuff(const std::string& str);
  void addToOutBuff(const Buffer& buffer);

  void removeFromOutBuff(ssize_t bytes);

  bool hasDataToSend() const;

private:
  int _fd;
  // int _state;
  Buffer _inBuff;
  Buffer _outBuff;
  Request _request;
};

#endif
