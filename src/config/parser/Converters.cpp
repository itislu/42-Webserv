#include "Converters.hpp"
#include "libftpp/array.hpp"
#include <cstddef>
#include <libftpp/ctype.hpp>
#include <libftpp/string.hpp>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utils/convert.hpp>

namespace config {
namespace convert {

std::size_t toMaxBodySize(const std::string& str)
{
  const std::size_t len = str.length();
  const char suffix = str[len - 1];

  std::size_t multiplier = 1;
  const unsigned long multi = 1024;
  std::string number;
  if (ft::isdigit(suffix)) {
    number = str;
  } else {
    if (suffix == 'b' || suffix == 'B') {
      multiplier = 1;
    } else if (suffix == 'k' || suffix == 'K') {
      multiplier = multi;
    } else if (suffix == 'm' || suffix == 'M') {
      multiplier = multi * multi;
    } else if (suffix == 'g' || suffix == 'G') {
      multiplier = multi * multi * multi;
    } else {
      throw std::invalid_argument("max_body_size: has invalid multiplier");
    }
    number = str.substr(0, len - 1);
  }
  std::size_t value = 0;
  std::istringstream sstream(number);
  sstream >> value;

  if (sstream.fail() || !sstream.eof() || number[0] == '-') {
    throw std::invalid_argument("max_body_size: invalid number: " + number);
  }

  if (value > std::numeric_limits<std::size_t>::max() / multiplier) {
    throw std::overflow_error("max_body_size: value too large");
  }

  return value * multiplier;
}

// 300 - 599
int toCode(const std::string& str)
{
  const int code = utils::toNumber<int>(str);
  const int minCode = 300;
  const int maxCode = 599;
  if (code >= minCode && code <= maxCode) {
    return code;
  }
  throw std::invalid_argument("invalid errorcode: " + str +
                              " (must be between 300 and 599)");
}

// 301, 302, 303, 307, 308
int toRedirectCode(const std::string& str)
{
  const int code = utils::toNumber<int>(str);
  static const ft::array<int, 5> allowed = { 301, 302, 303, 307, 308 };
  static const int size = sizeof(allowed) / sizeof(allowed[0]);

  for (int i = 0; i < size; ++i) {
    if (code == allowed[i]) {
      return code;
    }
  }
  throw std::invalid_argument("invalid redirect code: " + str +
                              " (allowed: 301, 302, 303, 307, 308)");
}

bool toBool(const std::string& str)
{
  const std::string lower = ft::to_lower(str);
  if (lower == "1" || lower == "true" || lower == "yes" || lower == "on") {
    return true;
  }
  if (lower == "0" || lower == "false" || lower == "no" || lower == "off") {
    return false;
  }
  throw std::runtime_error("invalid bool: " + str);
}

bool isMethod(const std::string& str)
{
  return (str == "GET" || str == "POST" || str == "DELETE");
}

} // namespace convert
} // namespace config
