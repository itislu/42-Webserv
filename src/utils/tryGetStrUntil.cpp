#include "tryGetStrUntil.hpp"
#include <algorithm>
#include <cstddef>
#include <stdexcept>

std::string tryGetStrUntil(const std::vector<unsigned char> buffer,
                           std::size_t iStart,
                           const std::vector<unsigned char> tokenSequenze)
{
  std::vector<unsigned char>::const_iterator it =
    std::search(buffer.begin() + iStart,
                buffer.end(),
                tokenSequenze.begin(),
                tokenSequenze.end());

  if (it != buffer.end()) {
    std::string str(buffer.begin() + iStart, it);
    return str;
  }
  throw std::out_of_range("Token not found");
}
