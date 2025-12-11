#include "CgiContext.hpp"

#include <http/statesCgi/executeCgi/ExecuteCgi.hpp>
#include <http/statesCgi/processCgiResponse/ProcessCgiResponse.hpp>
#include <utils/Pipe.hpp>
#include <utils/state/StateHandler.hpp>

#include <cstddef>

/* ************************************************************************** */
// PUBLIC

CgiContext::CgiContext(Client* client)
  : _client(client)
  , _shExecCgi(this)
  , _shProcessCgiResponse(this)
  , _contentLengthAvailable(false)
  , _contentLength(0)
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

void CgiContext::setContentLengthAvailable()
{
  _contentLengthAvailable = true;
}

void CgiContext::setContentLength(std::size_t value)
{
  _contentLength = value;
}

/* ************************************************************************** */
// PRIVATE
