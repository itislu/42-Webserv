// IWYU pragma: private; include "testUtils.hpp"
#pragma once
#ifndef TEST_UTILS
#include "testUtils.hpp"
#endif

#include <cstddef>
#include <string>

namespace testUtils {

// NOLINTBEGIN(cppcoreguidelines-avoid-c-arrays)
template<std::size_t N>
std::string makeString(const char (&arr)[N])
{
  if (arr[N - 1] == '\0') {
    return std::string(arr, N - 1);
  }
  return std::string(arr, N);
}
// NOLINTEND(cppcoreguidelines-avoid-c-arrays)

} // namespace testUtils
