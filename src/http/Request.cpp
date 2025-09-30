#include "Request.hpp"
#include <cstddef>

/* ************************************************************************** */
// INIT

const request::MethodMap Request::_methodMap[_methods] = {
  { "GET", request::GET },
  { "POST", request::POST },
  { "DELETE", request::DELETE },
};

/* ************************************************************************** */
// PUBLIC

Request::Request() {}

Request::~Request() {}

Request::Request(const Request& other)
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

request::Method Request::getMethod() const
{
  return _method;
}

void Request::setMethod(request::Method method)
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

request::Method Request::strToMethod(std::string& strMethod)
{

  for (std::size_t i = 0; i < _methods; i++) {
    if (_methodMap[i].methodStr == strMethod) {
      return _methodMap[i].method;
    }
  }
  return request::UNDEFINED;
}

/* ************************************************************************** */
// PRIVATE
