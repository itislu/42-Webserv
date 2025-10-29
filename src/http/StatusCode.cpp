#include "StatusCode.hpp"

#include <cassert>
#include <cstddef>
#include <ostream>
#include <sstream>
#include <string>

/* ************************************************************************** */
// INIT

const StatusCode::CodeEntry StatusCode::_codeMap[_codes] = {
  { Ok, "Ok" },
  { BadRequest, "Bad Request" },
  { Unauthorized, "Unauthorized" }
};

/* ************************************************************************** */
// PUBLIC

StatusCode::StatusCode()
  : _code(Ok)
  , _reason(_codeMap[0].reason)
{
}

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
StatusCode::StatusCode(Code code)
  : _code(code)
  , _reason(NULL)
{
  for (int i = 0; i < _codes; i++) {
    if (code == _codeMap[i].code) {
      _reason = _codeMap[i].reason;
      return;
    }
  }
  assert(false && "StatusCode: code out of range");
}
// NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)

StatusCode::Code StatusCode::getCode() const
{
  return _code;
}
const char* StatusCode::getReason() const
{
  return _reason;
}

std::string StatusCode::toString()
{
  std::stringstream oss;
  oss << _code << " " << _reason;
  return oss.str();
}

std::ostream& operator<<(std::ostream& out, const StatusCode& statuscode)
{
  out << statuscode.getCode() << " " << statuscode.getReason();
  return out;
}

/* ************************************************************************** */
// PRIVATE
