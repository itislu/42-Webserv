#include "Request.hpp"
#include "http/Uri.hpp"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <string>

/* ************************************************************************** */
// INIT

const Request::MethodMap Request::_methodMap[_methods] = {
  { "GET", Request::GET },
  { "POST", Request::POST },
  { "DELETE", Request::DELETE },
};

const std::size_t Request::MAX_METHOD_LEN = Request::_getMaxMethodLen();

/* ************************************************************************** */
// PUBLIC

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

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
Request::Method Request::strToMethod(std::string& strMethod)
{
  for (std::size_t i = 0; i < _methods; i++) {
    if (_methodMap[i].methodStr == strMethod) {
      return _methodMap[i].method;
    }
  }
  return Request::UNDEFINED;
}
// NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)

/* ************************************************************************** */
// PRIVATE

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
std::size_t Request::_getMaxMethodLen() throw()
{
  std::size_t maxLen = 0;
  for (std::size_t i = 0; i < _methods; i++) {
    maxLen = std::max(maxLen, strlen(_methodMap[i].methodStr));
  }
  return maxLen;
}
// NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)
