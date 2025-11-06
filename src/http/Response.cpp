#include "Response.hpp"

#include <fstream>
#include <http/Headers.hpp>
#include <http/StatusCode.hpp>

#include <string>

/* ************************************************************************** */
// PUBLIC

const std::string& Response::getVersion() const
{
  return _version;
}

void Response::setVersion(const std::string& version)
{
  _version = version;
}

void Response::setStatusCode(StatusCode::Code code)
{
  _statuscode = code;
}

const StatusCode& Response::getStatusCode() const
{
  return _statuscode;
}

Headers& Response::getHeaders()
{
  return _headers;
}

std::ifstream& Response::getBody()
{
  return _body;
}

/* ************************************************************************** */
// PRIVATE
