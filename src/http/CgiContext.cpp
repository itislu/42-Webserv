#include <sys/types.h>

#include "CgiContext.hpp"

#include <http/statesCgi/executeCgi/ExecuteCgi.hpp>
#include <http/statesCgi/processCgiResponse/ProcessCgiResponse.hpp>
#include <utils/Pipe.hpp>
#include <utils/process/ChildProcessManager.hpp>
#include <utils/state/StateHandler.hpp>

#include <cstddef>

/* ************************************************************************** */
// PUBLIC

CgiContext::CgiContext(Client* client)
  : _client(client)
  , _shExecCgi(this)
  , _shProcessCgiResponse(this)
  , _childPid(-1)
  , _contentLength(0)
  , _contentLengthAvailable(false)
  , _cgiReadEventHandlerReceivedPollHupErr(false)
  , _cgiWriteEventHandlerReceivedPollHupErr(false)
  , _timeoutRead(false)
  , _timeoutWrite(false)
{
  _shExecCgi.setState<ExecuteCgi>();
  _shProcessCgiResponse.setState<ProcessCgiResponse>();
}

Client* CgiContext::getClient()
{
  return _client;
}

StateHandler<CgiContext>& CgiContext::getShExecCgi()
{
  return _shExecCgi;
}

StateHandler<CgiContext>& CgiContext::getShProcessCgiResponse()
{
  return _shProcessCgiResponse;
}

bool CgiContext::contentLengthAvailable() const
{
  return _contentLengthAvailable;
}

std::size_t CgiContext::getContentLength() const
{
  return _contentLength;
}

Pipe& CgiContext::getPipeClientToCgi()
{
  return _pipeClientToCgi;
}

Pipe& CgiContext::getPipeCgiToClient()
{
  return _pipeCgiToClient;
}

void CgiContext::setChildPid(pid_t pid)
{
  _childPid = pid;
  ChildProcessManager::getInstance().addChild(pid);
}

pid_t CgiContext::getChildPid() const
{
  return _childPid;
}

void CgiContext::setContentLengthAvailable()
{
  _contentLengthAvailable = true;
}

void CgiContext::setContentLength(std::size_t value)
{
  _contentLength = value;
}

void CgiContext::setCgiReadEventHandlerReceivedPollHupErr(bool value)
{
  _cgiReadEventHandlerReceivedPollHupErr = value;
}

void CgiContext::setCgiWriteEventHandlerReceivedPollHupErr(bool value)
{
  _cgiWriteEventHandlerReceivedPollHupErr = value;
}

bool CgiContext::cgiReadEventHandlerReceivedPollHupErr() const
{
  return _cgiReadEventHandlerReceivedPollHupErr;
}

bool CgiContext::cgiWriteEventHandlerReceivedPollHupErr() const
{
  return _cgiWriteEventHandlerReceivedPollHupErr;
}

void CgiContext::setTimeoutRead(bool value)
{
  _timeoutRead = value;
}

void CgiContext::setTimeoutWrite(bool value)
{
  _timeoutWrite = value;
}

bool CgiContext::timeoutRead() const
{
  return _timeoutRead;
}

bool CgiContext::timeoutWrite() const
{
  return _timeoutWrite;
}

/* ************************************************************************** */
// PRIVATE
