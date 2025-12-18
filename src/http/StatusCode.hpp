#pragma once
#ifndef STATUS_CODE_HPP
#define STATUS_CODE_HPP

#include <libftpp/array.hpp>

#include <ostream>
#include <string>

/* ************************************************************************** */
class StatusCode
{
public:
  enum Code
  {
    Ok = 200,
    Created = 201,
    NoContent = 204,
    BadRequest = 400,
    Unauthorized = 401,
    Forbidden = 403,
    NotFound = 404,
    MethodNotAllowed = 405,
    NotAcceptable = 406,
    RequestTimeout = 408,
    Conflict = 409,
    LengthRequired = 411,
    ContentTooLarge = 413,
    UriTooLong = 414,
    MisdirectedRequest = 421,
    RequestHeaderFieldsTooLarge = 431,
    InternalServerError = 500,
    NotImplemented = 501,
    BadGateway = 502,
    HttpVersionNotSupported = 505
  };

  StatusCode();
  explicit StatusCode(Code code);
  StatusCode& operator=(Code code);
  bool operator==(Code code) const;
  bool operator!=(Code code) const;

  Code getCode() const;
  const char* getReason() const;
  std::string toString() const;

  bool is2xxCode() const;

private:
  void _findReason();

  struct CodeEntry;
  static const int _codes = 20;
  static const ft::array<CodeEntry, _codes> _codeMap;

  Code _code;
  const char* _reason;
};

struct StatusCode::CodeEntry
{
  Code code;
  const char* reason;
};

std::ostream& operator<<(std::ostream& out, const StatusCode& statuscode);

#endif
