#include "Request.hpp"

#include <http/Headers.hpp>
#include <http/Uri.hpp>
#include <libftpp/array.hpp>
#include <libftpp/utility.hpp>
#include <utils/Buffer.hpp>

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <sstream>
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

Headers& Request::getHeaders()
{
  return _headers;
}

Buffer& Request::getBody()
{
  return _body;
}

std::string Request::toString()
{
  std::stringstream oss;
  oss << "{\n";
  oss << "  \"method\": \"" << _methodToString() << "\",\n";
  oss << "  \"uri\": \n" << _uri.toString() << ",\n";
  oss << "  \"version\": \"" << _version << "\",\n";
  oss << "  \"headers\": \n{\n";
  oss << _headers.toLogString();
  oss << "  }\n";
  oss << "}\n";
  return oss.str();
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

/* TODO: need this to be public */
std::string Request::_methodToString() const
{
  switch (_method) {
    case UNDEFINED:
      return "UNDEFINED";
    case GET:
      return "GET";
    case POST:
      return "POST";
    case DELETE:
      return "DELETE";
  }
  FT_UNREACHABLE();
}
