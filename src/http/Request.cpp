#include "Request.hpp"

#include <http/Uri.hpp>
#include <libftpp/array.hpp>

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <string>

/* ************************************************************************** */
// INIT

const ft::array<Request::MethodMap, Request::_methods> Request::_methodMap = { {
  { "GET", Request::GET },
  { "POST", Request::POST },
  { "DELETE", Request::DELETE },
} };

const std::size_t Request::MaxMethodLen = Request::_getMaxMethodLen();

/* ************************************************************************** */
// PUBLIC

Request::Request()
  : _method(UNDEFINED)
{
}

Request::Method Request::getMethod() const
{
  return _method;
}

void Request::setMethod(Request::Method method)
{
  _method = method;
}

const Uri& Request::getUri() const
{
  return _uri;
}

void Request::setUri(const Uri& uri)
{
  _uri = uri;
}

const std::string& Request::getVersion() const
{
  return _version;
}

void Request::setVersion(const std::string& version)
{
  _version = version;
}

Request::Method Request::strToMethod(const std::string& strMethod)
{
  for (std::size_t i = 0; i < _methodMap.size(); i++) {
    if (_methodMap[i].methodStr == strMethod) {
      return _methodMap[i].method;
    }
  }
  return Request::UNDEFINED;
}

Request::HeaderMap& Request::getHeaders()
{
  return _headers;
}

/* ************************************************************************** */
// PRIVATE

std::size_t Request::_getMaxMethodLen() throw()
{
  std::size_t maxLen = 0;
  for (std::size_t i = 0; i < _methodMap.size(); i++) {
    maxLen = std::max(maxLen, std::strlen(_methodMap[i].methodStr));
  }
  return maxLen;
}
