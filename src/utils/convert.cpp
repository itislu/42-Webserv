#include "convert.hpp"

#include <cstddef>
#include <ios>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>

namespace utils {

// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
// NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
std::string addrToString(const sockaddr_storage& addr)
{
  if (addr.ss_family == AF_INET) {
    const sockaddr_in& ipv4Addr = reinterpret_cast<const sockaddr_in&>(addr);
    const unsigned char* const bytes =
      reinterpret_cast<const unsigned char*>(&ipv4Addr.sin_addr);
    const unsigned numOctets = 4;
    std::ostringstream oss;
    for (unsigned i = 0; i < numOctets; ++i) {
      if (i > 0) {
        oss << '.';
      }
      oss << static_cast<unsigned>(bytes[i]);
    }
    return oss.str();
  }

  if (addr.ss_family == AF_INET6) {
    const sockaddr_in6& ipv6Addr = reinterpret_cast<const sockaddr_in6&>(addr);
    const in6_addr& ipv6 = ipv6Addr.sin6_addr;
    const unsigned numGroups = 8;
    const unsigned byteSize = 8;
    std::ostringstream oss;
    oss << std::hex;
    for (std::size_t i = 0; i < numGroups; ++i) {
      if (i > 0) {
        oss << ':';
      }
      // Reconstruct 16-bit group from 8-bit array.
      const unsigned topByte = ipv6.s6_addr[i * 2];
      const unsigned botByte = ipv6.s6_addr[(i * 2) + 1];
      oss << ((topByte << byteSize) | botByte);
    }
    return oss.str();
  }

  throw std::runtime_error("addrToString: unknown address family");
}
// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)
// NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

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
