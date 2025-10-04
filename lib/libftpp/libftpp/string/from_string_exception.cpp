#include "libftpp/exception.hpp"
#include "libftpp/string.hpp"
#include "libftpp/utility.hpp"
#include <algorithm>
#include <string>
#include <typeinfo>

namespace ft {

from_string_exception::from_string_exception(const std::string& msg,
                                             const std::string& input,
                                             const std::type_info& type_id)
    : ft::exception(msg, "ft::from_string"),
      _input(input),
      _type_id(&type_id)
{}

from_string_exception::from_string_exception(
    const from_string_exception& other) throw()
    : ft::exception(other),
      _type_id(other._type_id)
{
	try {
		_input = other._input;
	}
	catch (...) {
		// EMPTY: Exception objects should be nothrow constructable.
	}
}

from_string_exception::~from_string_exception() throw() {}

from_string_exception&
from_string_exception::operator=(from_string_exception other) throw()
{
	swap(other);
	return *this;
}

const std::string& from_string_exception::input() const throw()
{
	return _input;
}

const std::type_info& from_string_exception::type_id() const throw()
{
	return *_type_id;
}

void from_string_exception::swap(from_string_exception& other) throw()
{
	using std::swap;
	swap(_input, other._input);
	swap(_type_id, other._type_id);
}

from_string_range_exception::from_string_range_exception(
    const std::string& input,
    const std::type_info& type_id)
    : from_string_exception("numerical result out of range ("
                                + ft::demangle(type_id.name()) + "): \"" + input
                                + "\"",
                            input,
                            type_id)
{}

from_string_invalid_exception::from_string_invalid_exception(
    const std::string& input,
    const std::type_info& type_id)
    : from_string_exception("could not convert to "
                                + ft::demangle(type_id.name()) + ": \"" + input
                                + "\"",
                            input,
                            type_id)
{}

} // namespace ft
