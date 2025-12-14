#include "Request.hpp"

#include <http/Headers.hpp>
#include <http/Uri.hpp>
#include <libftpp/array.hpp>
#include <libftpp/utility.hpp>
#include <utils/buffer/SmartBuffer.hpp>

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

Request::Request(const Request& other)
  : _method(UNDEFINED)
  , _uri(other._uri)
  , _version(other._version)
  , _headers(other._headers)
{
}

Request& Request::operator=(const Request& other)
{
  _method = other._method;
  _uri = other._uri;
  _version = other._version;
  _headers = other._headers;
  // body can not be copied
  return *this;
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

Request::Method Request::getMethod() const
{
  return _method;
}

const Uri& Request::getUri() const
{
  return _uri;
}

const std::string& Request::getVersion() const
{
  return _version;
}

Headers& Request::getHeaders()
{
  return _headers;
}

SmartBuffer& Request::getBody()
{
  return _body;
}

Headers& Request::getTrailers()
{
  return _trailers;
}

void Request::setMethod(Request::Method method)
{
  _method = method;
}

void Request::setUri(const Uri& uri)
{
  _uri = uri;
}

void Request::setVersion(const std::string& version)
{
  _version = version;
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

/* ***************************************************************************/
// PRIVATE

std::size_t Request::_getMaxMethodLen() throw()
{
  std::size_t maxLen = 0;
  for (std::size_t i = 0; i < _methodMap.size(); i++) {
    maxLen = std::max(maxLen, std::strlen(_methodMap[i].methodStr));
  }
  return maxLen;
}

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
