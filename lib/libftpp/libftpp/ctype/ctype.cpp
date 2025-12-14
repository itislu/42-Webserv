#include "libftpp/ctype.hpp"
#include <cctype>

namespace ft {

/* Character classification */

bool isalnum(char ch)
{
	return std::isalnum(static_cast<unsigned char>(ch)) != 0;
}

bool isalpha(char ch)
{
	return std::isalpha(static_cast<unsigned char>(ch)) != 0;
}

bool islower(char ch)
{
	return std::islower(static_cast<unsigned char>(ch)) != 0;
}

bool isupper(char ch)
{
	return std::isupper(static_cast<unsigned char>(ch)) != 0;
}

bool isdigit(char ch)
{
	return std::isdigit(static_cast<unsigned char>(ch)) != 0;
}

bool isxdigit(char ch)
{
	return std::isxdigit(static_cast<unsigned char>(ch)) != 0;
}

bool iscntrl(char ch)
{
	return std::iscntrl(static_cast<unsigned char>(ch)) != 0;
}

bool isgraph(char ch)
{
	return std::isgraph(static_cast<unsigned char>(ch)) != 0;
}

bool isspace(char ch)
{
	return std::isspace(static_cast<unsigned char>(ch)) != 0;
}

bool isblank(char ch) { return ch == ' ' || ch == '\t'; }

bool isprint(char ch)
{
	return std::isprint(static_cast<unsigned char>(ch)) != 0;
}

bool ispunct(char ch)
{
	return std::ispunct(static_cast<unsigned char>(ch)) != 0;
}

/* Character manipulation */

char tolower(char ch)
{
	return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
}

char toupper(char ch)
{
	return static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
}

} // namespace ft
