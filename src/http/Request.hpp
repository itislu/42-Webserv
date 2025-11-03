#pragma once
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <http/Uri.hpp>
#include <libftpp/array.hpp>

#include <cstddef>
#include <map>
#include <string>

/* ************************************************************************** */
class Request
{
public:
  typedef std::map<std::string, std::string> HeaderMap;

  enum Method
  {
    UNDEFINED,
    GET,
    POST,
    DELETE
  };

  Request();

  static const std::size_t MaxMethodLen;
  static Method strToMethod(const std::string& strMethod);

  Method getMethod() const;
  void setMethod(Method method);

  const Uri& getUri() const;
  void setUri(const Uri& uri);

  const std::string& getVersion() const;
  void setVersion(const std::string& version);

  HeaderMap& getHeaders();

private:
  struct MethodMap;
  static const int _methods = 3;
  static const ft::array<MethodMap, _methods> _methodMap;
  static std::size_t _getMaxMethodLen() throw();

  Method _method;
  Uri _uri;
  std::string _version;
  HeaderMap _headers;
};

struct Request::MethodMap
{
  const char* methodStr;
  Request::Method method;
};

#endif
