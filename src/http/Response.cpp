#include "Response.hpp"

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
  _statuscode = StatusCode(code);
}

StatusCode& Response::getStatusCode()
{
  return _statuscode;
}

/* ************************************************************************** */
// PRIVATE
