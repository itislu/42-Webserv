#include "libftpp/string.hpp"
#include <algorithm>
#include <cctype>
#include <string>

static char to_lowercase(unsigned char c) {
  return static_cast<char>(std::tolower(c));
}

namespace ft {

std::string &to_lower(std::string &str) {
  std::transform(str.begin(), str.end(), str.begin(), to_lowercase);
  return str;
}

std::string to_lower(const std::string &str) {
  std::string newStr = str;
  std::transform(str.begin(), str.end(), newStr.begin(), to_lowercase);
  return str;
}

} // namespace ft
