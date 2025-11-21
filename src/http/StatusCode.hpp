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
    BadRequest = 400,
    Unauthorized = 401,
    InternalServerError = 500
  };

  StatusCode();
  explicit StatusCode(Code code);
  StatusCode& operator=(Code code);
  bool operator==(Code code) const;
  bool operator!=(Code code) const;

  Code getCode() const;
  const char* getReason() const;
  std::string toString() const;

private:
  void _findReason();

  struct CodeEntry;
  static const int _codes = 5;
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
