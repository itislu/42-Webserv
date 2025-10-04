#include "libftpp/source_location.hpp"
#include <sstream>
#include <stdint.h>
#include <string>

namespace ft {

source_location::source_location() throw()
    : _line(0),
      _file_name(""),
      _function_name("")
{}

source_location::source_location(uint_least32_t line,
                                 const char* file_name,
                                 const char* function_name) throw()
    : _line(line),
      _file_name(file_name),
      _function_name(function_name)
{}

std::string source_location::format() const
{
	std::ostringstream os;

	os << _file_name << ":" << _line << ": " << _function_name;
	return os.str();
}

uint_least32_t source_location::line() const throw() { return _line; }

const char* source_location::file_name() const throw() { return _file_name; }

const char* source_location::function_name() const throw()
{
	return _function_name;
}

} // namespace ft
