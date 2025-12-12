#pragma once
#ifndef CGI_CONTEXT_HPP
#define CGI_CONTEXT_HPP

#include <utils/Pipe.hpp>
#include <utils/state/StateHandler.hpp>

#include <cstddef>
#include <map>
#include <string>

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

  void setContentLengthAvailable();
  void setContentLength(std::size_t value);

  void setCgiReadEventHandlerReceivedPollHupErr(bool value);
  void setCgiWriteEventHandlerReceivedPollHupErr(bool value);
  bool cgiReadEventHandlerReceivedPollHupErr() const;
  bool cgiWriteEventHandlerReceivedPollHupErr() const;

private:
  Client* _client;
  StateHandler<CgiContext> _shExecCgi;
  StateHandler<CgiContext> _shProcessCgiResponse;

  std::map<std::string, std::string> _env;

  Pipe _pipeClientToCgi;
  Pipe _pipeCgiToClient;

  std::size_t _contentLength;
  bool _contentLengthAvailable;
  bool _cgiReadEventHandlerReceivedPollHupErr;
  bool _cgiWriteEventHandlerReceivedPollHupErr;
};

#endif
