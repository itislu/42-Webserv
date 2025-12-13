#include "StatusCode.hpp"

#include <libftpp/array.hpp>
#include <libftpp/utility.hpp>

#include <cassert>
#include <cstddef>
#include <ostream>
#include <sstream>
#include <string>

/* ************************************************************************** */
// INIT

const ft::array<StatusCode::CodeEntry, StatusCode::_codes>
  StatusCode::_codeMap = {
    { { Ok, "OK" },
      { Created, "Created" },
      { BadRequest, "Bad Request" },
      { Unauthorized, "Unauthorized" },
      { Forbidden, "Forbidden" },
      { NotFound, "Not Found" },
      { MethodNotAllowed, "Method Not Allowed" },
      { NotAcceptable, "Not Acceptable" },
      { RequestTimeout, "Request Timeout" },
      { LengthRequired, "Length Required" },
      { ContentTooLarge, "Content Too Large" },
      { UriTooLong, "URI Too Long" },
      { MisdirectedRequest, "Misdirected Request" },
      { RequestHeaderFieldsTooLarge, "Request Header Fields Too Large" },
      { InternalServerError, "Internal Server Error" },
      { NotImplemented, "Not Implemented" },
      { HttpVersionNotSupported, "HTTP Version Not Supported" } }
  };

/* ***************************************************************************/
// PUBLIC

StatusCode::StatusCode()
  : _code(Ok)
  , _reason(_codeMap[0].reason)
{
}

StatusCode::StatusCode(Code code)
  : _code(code)
  , _reason()
{
  _findReason();
}

StatusCode& StatusCode::operator=(Code code)
{
  _code = code;
  _findReason();
  return *this;
}

bool StatusCode::operator==(Code code) const
{
  return code == getCode();
}

bool StatusCode::operator!=(Code code) const
{
  return code != getCode();
}

StatusCode::Code StatusCode::getCode() const
{
  return _code;
}
const char* StatusCode::getReason() const
{
  return _reason;
}

std::string StatusCode::toString() const
{
  std::stringstream oss;
  oss << _code << " " << _reason;
  return oss.str();
}

std::ostream& operator<<(std::ostream& out, const StatusCode& statuscode)
{
  out << statuscode.toString();
  return out;
}

/* ************************************************************************** */
// PRIVATE

void StatusCode::_findReason()
{
  for (std::size_t i = 0; i < _codeMap.size(); i++) {
    if (_code == _codeMap[i].code) {
      _reason = _codeMap[i].reason;
      return;
    }
  }
  assert(false && "StatusCode: code out of range");
  FT_UNREACHABLE();
}
