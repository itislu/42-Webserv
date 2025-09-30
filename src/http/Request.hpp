#pragma once
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "http/Uri.hpp"
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
  ~Request();
  Request(const Request& other);
  Request& operator=(const Request& other);

  Method getMethod() const;
  void setMethod(Method method);
  Uri& getUri();
  std::string getVersion() const;
  void setVersion(std::string& version);

  static Method strToMethod(std::string& strMethod);

private:
  struct MethodMap;
  static const int _methods = 3;
  static const MethodMap _methodMap[_methods];

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
