#include "tryGetStrUntil.hpp"
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

// NOLINTBEGIN (clang-tidy cppcoreguidelines-narrowing-conversions)
std::string tryGetStrUntil(const std::vector<unsigned char>& buffer,
                           std::size_t iStart,
                           const std::vector<unsigned char>& tokenSequenze)
{
  std::vector<unsigned char>::const_iterator iter =
    std::search(buffer.begin() + iStart,
                buffer.end(),
                tokenSequenze.begin(),
                tokenSequenze.end());

  if (iter != buffer.end()) {
    std::string str(buffer.begin() + iStart, iter);
    return str;
  }
  throw std::out_of_range("Token not found");
}
// NOLINTEND
