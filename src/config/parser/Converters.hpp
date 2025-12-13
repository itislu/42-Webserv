#ifndef CONVERTERS_HPP
#define CONVERTERS_HPP

#include <cstddef>
#include <string>

namespace config {
namespace convert {

std::size_t toMaxBodySize(const std::string& str);
int toCode(const std::string& str);
bool toBool(const std::string& str);
bool isMethod(const std::string& str);

} // namespace convert
} // namespace config

#endif
