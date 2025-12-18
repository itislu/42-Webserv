#pragma once
#ifndef CGI_CONTEXT_HPP
#define CGI_CONTEXT_HPP

#include <sys/types.h>

#include <utils/Pipe.hpp>
#include <utils/state/StateHandler.hpp>

#include <cstddef>

class Client;

class CgiContext
{
public:
  explicit CgiContext(Client* client);

  Client* getClient();
  StateHandler<CgiContext>& getShExecCgi();
  StateHandler<CgiContext>& getShProcessCgiResponse();
  bool contentLengthAvailable() const;
  std::size_t getContentLength() const;
  Pipe& getPipeClientToCgi();
  Pipe& getPipeCgiToClient();

  void setChildPid(pid_t pid);
  pid_t getChildPid() const;

  void setContentLengthAvailable();
  void setContentLength(std::size_t value);

  void setCgiReadEventHandlerReceivedPollHupErr(bool value);
  void setCgiWriteEventHandlerReceivedPollHupErr(bool value);
  bool cgiReadEventHandlerReceivedPollHupErr() const;
  bool cgiWriteEventHandlerReceivedPollHupErr() const;

  void setTimeoutRead(bool value);
  void setTimeoutWrite(bool value);
  bool timeoutRead() const;
  bool timeoutWrite() const;

private:
  Client* _client;
  StateHandler<CgiContext> _shExecCgi;
  StateHandler<CgiContext> _shProcessCgiResponse;

  Pipe _pipeClientToCgi;
  Pipe _pipeCgiToClient;

  pid_t _childPid;
  std::size_t _contentLength;
  bool _contentLengthAvailable;
  bool _cgiReadEventHandlerReceivedPollHupErr;
  bool _cgiWriteEventHandlerReceivedPollHupErr;
  bool _timeoutRead;
  bool _timeoutWrite;
};

#endif
