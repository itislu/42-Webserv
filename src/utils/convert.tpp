// IWYU pragma: private; include "utils/convert.hpp"
#pragma once
#ifndef UTILS_CONVERT_HPP
#include "convert.hpp"
#endif

#include <libftpp/expected.hpp>
#include <libftpp/string.hpp>

#include <new>
#include <stdexcept>
#include <string>

namespace utils {

template<typename To>
To toNumber(const std::string& str)
{
  std::string::size_type validChars = 0;
  const ft::expected<To, ft::from_string_exception> number =
    ft::from_string<To>(str, std::nothrow, &validChars);

  if (!number.has_value()) {
    throw std::invalid_argument("invalid number: " + str);
  }
  if (validChars != str.size()) {
    throw std::invalid_argument("excess characters after number: " + str);
  }

  return *number;
}

} // namespace utils
