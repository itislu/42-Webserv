#include "convert.hpp"

#include <netinet/in.h>
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

in_port_t toPort(const std::string& str)
try {
  const in_port_t port = utils::toNumber<in_port_t>(str);
  if (port == 0) {
    throw std::invalid_argument("cannot be 0");
  }
  return port;
} catch (const std::invalid_argument& e) {
  throw std::invalid_argument(std::string("invalid port: ") + e.what());
}

} // namespace utils
