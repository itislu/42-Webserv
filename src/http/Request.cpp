#include "Request.hpp"
#include "http/Uri.hpp"
#include <cstddef>
#include <string>

/* ************************************************************************** */
// INIT

const Request::MethodMap Request::_methodMap[_methods] = {
  { "GET", Request::GET },
  { "POST", Request::POST },
  { "DELETE", Request::DELETE },
};

/* ************************************************************************** */
// PUBLIC

Request::Request()
  : _method(UNDEFINED)
{
}

Request::~Request() {}

Request::Request(const Request& other)
  : _method(other._method)
  , _uri(other._uri)
  , _version(other._version)
{
  *this = other;
}

Request& Request::operator=(const Request& other)
{
  if (this != &other) {
    // todo copy logic
  }
  return *this;
}

Request::Method Request::getMethod() const
{
  return _method;
}

void Request::setMethod(Request::Method method)
{
  _method = method;
}

Uri& Request::getUri()
{
  return _uri;
}

std::string Request::getVersion() const
{
  return _version;
}

void Request::setVersion(std::string& version)
{
  _version = version;
}

// NOLINTBEGIN (clang-tidy cppcoreguidelines-pro-bounds-constant-array-index)
Request::Method Request::strToMethod(std::string& strMethod)
{
  for (std::size_t i = 0; i < _methods; i++) {
    if (_methodMap[i].methodStr == strMethod) {
      return _methodMap[i].method;
    }
  }
  return Request::UNDEFINED;
}
// NOLINTEND

/* ************************************************************************** */
// PRIVATE
