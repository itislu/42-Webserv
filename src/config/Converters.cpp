#include "Converters.hpp"
#include <cstddef>
#include <libftpp/ctype.hpp>
#include <libftpp/string.hpp>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

namespace config {
namespace convert {

std::size_t toSizeT(const std::string& str)
{
  std::istringstream sstream(str);
  std::size_t value = 0;
  sstream >> value;

  if (sstream.fail() || !sstream.eof()) {
    throw std::invalid_argument("invalid number: " + str);
  }

  return value;
}

long toLong(const std::string& str)
{
  std::istringstream sstream(str);
  long value = 0;
  sstream >> value;

  if (sstream.fail() || !sstream.eof()) {
    throw std::invalid_argument("invalid long: " + str);
  }

  return value;
}

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

int toInt(const std::string& str)
{
  std::istringstream sstream(str);
  int value = 0;
  sstream >> value;

  if (sstream.fail() || !sstream.eof()) {
    throw std::invalid_argument("invalid int: " + str);
  }

  return value;
}

// 65535
int toPort(const std::string& str)
{
  const int port = toInt(str);
  const int maxPort = 65535;
  if (port > 0 && port <= maxPort) {
    return port;
  }
  throw std::invalid_argument("invalid port: " + str);
}

// 300 - 599
int toCode(const std::string& str)
{
  const int code = toInt(str);
  const int minCode = 300;
  const int maxCode = 599;
  if (code >= minCode && code <= maxCode) {
    return code;
  }
  throw std::invalid_argument("invalid errorcode: " + str +
                              " (must be between 300 and 599)");
}

int hexToInt(char chr)
{
  if (chr >= 'A' && chr <= 'F') {
    return chr - 'A' + 10;
  }
  if (chr >= 'a' && chr <= 'f') {
    return chr - 'a' + 10;
  }
  if (chr >= '0' && chr <= '9') {
    return chr - '0';
  }
  return -1;
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
