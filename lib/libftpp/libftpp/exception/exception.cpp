#include "libftpp/exception.hpp"
#include "libftpp/optional.hpp"
#include "libftpp/source_location.hpp"
#include <algorithm>
#include <string>

namespace ft {

exception::exception(const std::string& error)
    : _error(error)
{
	_update_what_output();
}

exception::exception(const std::string& error, const ft::source_location& where)
    : _error(error),
      _where(where)
{
	_update_what_output();
}

exception::exception(const std::string& error, const std::string& who)
    : _error(error),
      _who(!who.empty() ? ft::optional<std::string>(who) : ft::nullopt)
{
	_update_what_output();
}

exception::exception(const std::string& error,
                     const ft::source_location& where,
                     const std::string& who)
    : _error(error),
      _where(where),
      _who(!who.empty() ? ft::optional<std::string>(who) : ft::nullopt)
{
	_update_what_output();
}

exception::exception(const exception& other) throw()
{
	try {
		_what_output = other._what_output;
	}
	catch (...) {
		// EMPTY: Exception objects should be nothrow constructable.
	}
	try {
		_error = other._error;
	}
	catch (...) {
		// EMPTY: Exception objects should be nothrow constructable.
	}
	try {
		_where = other._where;
	}
	catch (...) {
		// EMPTY: Exception objects should be nothrow constructable.
	}
	try {
		_who = other._who;
	}
	catch (...) {
		// EMPTY: Exception objects should be nothrow constructable.
	}
}

exception::~exception() throw() {}

exception& exception::operator=(exception other) throw()
{
	swap(other);
	return *this;
}

const char* exception::what() const throw() { return _what_output.c_str(); }

void exception::swap(exception& other) throw()
{
	using std::swap;
	swap(_what_output, other._what_output);
	swap(_error, other._error);
	swap(_where, other._where);
	swap(_who, other._who);
}

exception& exception::set_where(const ft::source_location& where)
{
	_where = where;
	_update_what_output();
	return *this;
}

exception& exception::set_who(const std::string& who)
{
	if (who.empty()) {
		_who.reset();
	}
	else {
		_who = who;
	}
	_update_what_output();
	return *this;
}

const std::string& exception::error() const throw() { return _error; }

const ft::optional<ft::source_location>& exception::where() const throw()
{
	return _where;
}

const ft::optional<std::string>& exception::who() const throw() { return _who; }

void exception::_update_what_output()
{
	_what_output.clear();
	if (_where.has_value()) {
		_what_output.append(_where->format());
		_what_output.append(": ");
	}
	if (_who.has_value()) {
		_what_output.append(*_who);
		_what_output.append(": ");
	}
	_what_output.append(_error);
}

void swap(exception& lhs, exception& rhs) throw() { lhs.swap(rhs); }

} // namespace ft
