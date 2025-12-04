#include "convert.hpp"

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

} // namespace utils
