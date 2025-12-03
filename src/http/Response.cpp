#include "Response.hpp"

#include <http/Headers.hpp>
#include <http/StatusCode.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
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

const ft::shared_ptr<IInBuffer>& Response::getBody() const
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

void Response::setBody(ft::shared_ptr<IInBuffer> body)
{
  _body = ft::move(body);
}

/* ************************************************************************** */
// PRIVATE
