#include "Response.hpp"

#include <http/Headers.hpp>
#include <http/StatusCode.hpp>
#include <libftpp/memory.hpp>
#include <utils/buffer/IInBuffer.hpp>

#include <string>

/* ************************************************************************** */
// PUBLIC

const std::string& Response::getVersion() const
{
  return _version;
}

const StatusCode& Response::getStatusCode() const
{
  return _statuscode;
}

ft::shared_ptr<IInBuffer>& Response::getBody()
{
  return _body;
}

Headers& Response::getHeaders()
{
  return _headers;
}

void Response::setVersion(const std::string& version)
{
  _version = version;
}

void Response::setStatusCode(StatusCode::Code code)
{
  _statuscode = code;
}

/* ************************************************************************** */
// PRIVATE
