#include "Converters.hpp"
#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>

std::size_t toSizeT(const std::string& str)
{
  std::stringstream sstream(str);
  std::size_t value = 0;
  sstream >> value;

  if (sstream.fail() || !sstream.eof()) {
    throw std::runtime_error("invalid number: " + str);
  }

  return value;
}

