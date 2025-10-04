#include "libftpp/string.hpp"
#include <ios>
#include <sstream>
#include <string>

namespace ft {

std::string to_string(bool v, std::ios::fmtflags fmt /*= std::ios::boolalpha*/)
{
	std::ostringstream oss;
	oss.flags(fmt);
	oss << v;
	return oss.str();
}

} // namespace ft
