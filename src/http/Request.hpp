#pragma once
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "http/Uri.hpp"

/* ************************************************************************** */
namespace request {
enum Method
{
  UNDEFINED,
  GET,
  POST,
  DELETE
};

typedef struct sMethodMap
{
  const char* methodStr;
  Method method;
} MethodMap;

}

/* ************************************************************************** */
class Request
{
public:
  Request();
  ~Request();
  Request(const Request& other);
  Request& operator=(const Request& other);

  request::Method getMethod() const;
  void setMethod(request::Method method);

  Uri& getUri();

  std::string getVersion() const;
  void setVersion(std::string& version);

  static request::Method strToMethod(std::string& strMethod);

private:
  static const int _methods = 3;
  static const request::MethodMap _methodMap[_methods];

  request::Method _method;
  Uri _uri;
  std::string _version;
};

#endif // REQUEST_HPP
