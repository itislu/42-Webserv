#include "libftpp/ctype.hpp"
#include "libftpp/string.hpp"
#include <algorithm>
#include <string>

namespace ft {

std::string& to_upper(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ft::toupper);
	return str;
}

std::string to_upper(const std::string& str)
{
	std::string newStr(str);
	return to_upper(newStr);
}

} // namespace ft
