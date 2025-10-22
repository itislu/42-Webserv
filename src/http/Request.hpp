#pragma once
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "http/Uri.hpp"
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

  Method getMethod() const;
  void setMethod(Method method);

  const Uri& getUri() const;
  void setUri(const Uri& uri);

  const std::string& getVersion() const;
  void setVersion(const std::string& version);

  static const std::size_t MAX_METHOD_LEN;
  static Method strToMethod(std::string& strMethod);

private:
  struct MethodMap;
  static const int _methods = 3;
  static const MethodMap _methodMap[_methods];
  static std::size_t _getMaxMethodLen() throw();

  Method _method;
  Uri _uri;
  std::string _version;
};

struct Request::MethodMap
{
  const char* methodStr;
  Request::Method method;
};

#endif // REQUEST_HPP
