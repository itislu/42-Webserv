#pragma once
#ifndef LIBFTPP_EXCEPTION_HPP
#	define LIBFTPP_EXCEPTION_HPP

#	include "libftpp/optional.hpp"
#	include "libftpp/source_location.hpp"
#	include <exception>
#	include <string>

namespace ft {

/**
 * @brief Exception class with context information
 *
 * The class inherits from `std::exception`, so it can be caught as such.
 *
 * @note All non-standard exception classes in libftpp inherit from this class.
 */
class exception : public std::exception {
public:
	explicit exception(const std::string& error);
	exception(const std::string& error, const ft::source_location& where);
	exception(const std::string& error, const std::string& who);
	exception(const std::string& error,
	          const ft::source_location& where,
	          const std::string& who);
	exception(const exception& other) throw();
	virtual ~exception() throw();
	exception& operator=(exception other) throw();

	/**
	 * @return An error message in the following format:
	 * `[where().format(): ][who(): ]<error()>`
	 */
	const char* what() const throw();
	void swap(exception& other) throw();

	exception& set_where(const ft::source_location& where);
	exception& set_who(const std::string& who);
	/**
	 * @return The unformatted error message, not including information from
	 * `where()` and `who()`
	 */
	const std::string& error() const throw();
	const ft::optional<ft::source_location>& where() const throw();
	const ft::optional<std::string>& who() const throw();

private:
	void _update_what_output();

	std::string _what_output;
	std::string _error;
	ft::optional<ft::source_location> _where;
	ft::optional<std::string> _who;
};

void swap(exception& lhs, exception& rhs) throw();

} // namespace ft

#endif // LIBFTPP_EXCEPTION_HPP
