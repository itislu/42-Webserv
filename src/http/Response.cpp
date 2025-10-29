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
  _statuscode = code;
}

const StatusCode& Response::getStatusCode() const
{
  return _statuscode;
}

/* ************************************************************************** */
// PRIVATE
