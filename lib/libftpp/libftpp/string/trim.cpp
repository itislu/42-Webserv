#include "libftpp/string.hpp"
#include <cctype>
#include <string>

namespace ft {

std::string& trim(std::string& str)
{
	std::string::iterator start = str.begin();
	while (start != str.end() && std::isspace(*start) != 0) {
		++start;
	}
	str.erase(str.begin(), start);
	if (str.empty()) {
		return str;
	}
	std::string::iterator end = str.end() - 1;
	while (std::isspace(*end) != 0) {
		--end;
	}
	str.erase(end + 1, str.end());
	return str;
}

std::string trim(const std::string& str)
{
	std::string new_str(str);
	trim(new_str);
	return new_str;
}

} // namespace ft

#ifdef TEST_STRING
#	include "libftpp/utility.hpp"
#	include <iostream>

int main()
{
	std::string tests[] = {"  Hello World  ",
	                       "   ",
	                       "Hello",
	                       "",
	                       " a ",
	                       "a ",
	                       " a",
	                       " ",
	                       "Hey there"};

	for (unsigned int i = 0; i < FT_COUNTOF(tests); ++i) {
		std::cout << "'" << ft::trim(tests[i]) << "'" << '\n';
	}
}
#endif // TEST_STRING
