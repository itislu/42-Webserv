#pragma once
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <http/Headers.hpp>
#include <http/Uri.hpp>
#include <utils/Buffer.hpp>

#include <cstddef>
#include <string>

/* ************************************************************************** */
class Request
{
public:
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

  Headers& getHeaders();

  Buffer& getBody();

  std::string toString();

private:
  struct MethodMap;
  static const int _methods = 3;
  static const MethodMap _methodMap[_methods];
  static std::size_t _getMaxMethodLen() throw();
  std::string _methodToString() const;

  Method _method;
  Uri _uri;
  std::string _version;
  Headers _headers;
  Buffer _body;
};

struct Request::MethodMap
{
  const char* methodStr;
  Request::Method method;
};

#endif
