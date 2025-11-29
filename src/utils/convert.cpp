#include "convert.hpp"

#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>

namespace utils {

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

} // namespace utils
