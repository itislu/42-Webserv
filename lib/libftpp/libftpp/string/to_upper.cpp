#include "libftpp/string.hpp"
#include <algorithm>
#include <cctype>
#include <string>

static char to_uppercase(unsigned char c)
{
	return static_cast<char>(std::toupper(c));
}

namespace ft {

std::string& to_upper(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), to_uppercase);
	return str;
}

std::string to_upper(const std::string& str)
{
	std::string newStr = str;
	std::transform(str.begin(), str.end(), newStr.begin(), to_uppercase);
	return newStr;
}

} // namespace ft
