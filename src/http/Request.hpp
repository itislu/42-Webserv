#pragma once
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <http/Headers.hpp>
#include <http/Uri.hpp>
#include <libftpp/array.hpp>
#include <utils/buffer/SmartBuffer.hpp>

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
  ~Request() {}
  Request(const Request& other);
  Request& operator=(const Request& other);

  static const std::size_t MaxMethodLen;
  static Method strToMethod(const std::string& strMethod);

  Method getMethod() const;
  std::string getStrMethod() const;
  const Uri& getUri() const;
  const std::string& getVersion() const;
  Headers& getHeaders();
  SmartBuffer& getBody();
  Headers& getTrailers();

  void setMethod(Method method);
  void setUri(const Uri& uri);
  void setVersion(const std::string& version);

  std::string toString() const;

private:
  struct MethodMap;
  static const int _methods = 3;
  static const ft::array<MethodMap, _methods> _methodMap;
  static std::size_t _getMaxMethodLen() throw();
  std::string _methodToString() const;

  Method _method;
  Uri _uri;
  std::string _version;
  Headers _headers;
  SmartBuffer _body;
  Headers _trailers;
};

struct Request::MethodMap
{
  const char* methodStr;
  Request::Method method;
};

#endif
