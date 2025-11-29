// IWYU pragma: private; include "utils/convert.hpp"
#pragma once
#ifndef UTILS_CONVERT_HPP
#include "convert.hpp"
#endif

#include <libftpp/utility.hpp>

#include <sstream>
#include <stdexcept>
#include <string>

namespace utils {

template<typename To>
To toNumber(const std::string& str)
{
  std::istringstream sstream(str);
  To value = 0;
  sstream >> value;

  if (sstream.fail() || !sstream.eof()) {
    throw std::invalid_argument("invalid " + ft::demangle(typeid(To)) + ": " +
                                str);
  }

  return value;
}

} // namespace utils
