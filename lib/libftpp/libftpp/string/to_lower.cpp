#include "libftpp/ctype.hpp"
#include "libftpp/string.hpp"
#include <algorithm>
#include <string>

namespace ft {

std::string& to_lower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ft::tolower);
	return str;
}

std::string to_lower(const std::string& str)
{
	std::string newStr(str);
	return to_lower(newStr);
}

} // namespace ft
