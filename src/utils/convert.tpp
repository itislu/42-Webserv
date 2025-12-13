// IWYU pragma: private; include "utils/convert.hpp"
#pragma once
#ifndef UTILS_CONVERT_HPP
#include "convert.hpp"
#endif

#include <libftpp/string.hpp>
#include <libftpp/utility.hpp>

#include <stdexcept>
#include <string>

namespace utils {

template<typename To>
To toNumber(const std::string& str)
try {
  std::string::size_type validChars = 0;
  const To number = ft::from_string<To>(str, &validChars);
  if (validChars != str.size()) {
    throw std::invalid_argument("excess characters after number: \"" + str +
                                "\"");
  }
  return number;
} catch (const ft::from_string_range_exception& e) {
  const std::string typeName = ft::demangle(e.type_id());
  throw std::invalid_argument("number out of range (" + typeName + "): " + str);
} catch (const ft::from_string_exception&) {
  throw std::invalid_argument("invalid number: " + str);
}

} // namespace utils
