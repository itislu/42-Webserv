#pragma once
#ifndef LIBFTPP_SOURCE_LOCATION_HPP
#	define LIBFTPP_SOURCE_LOCATION_HPP

#	include <stdint.h>
#	include <string>

#	if defined(__GNUC__) || defined(__clang__)
#		define FT_SOURCE_LOCATION_CURRENT()                               \
			(ft::source_location(__LINE__, __FILE__, __PRETTY_FUNCTION__))
#	elif defined(_MSC_VER)
#		define FT_SOURCE_LOCATION_CURRENT()                       \
			(ft::source_location(__LINE__, __FILE__, __FUNCSIG__))
#	else // defined(__GNUC__) || defined(__clang__)
#		define FT_SOURCE_LOCATION_CURRENT()                    \
			(ft::source_location(__LINE__, __FILE__, __func__))
#	endif // defined(__GNUC__) || defined(__clang__)

namespace ft {

/**
 * https://en.cppreference.com/w/cpp/utility/source_location
 */
class source_location {
public:
	source_location() throw();
	source_location(uint_least32_t line,
	                const char* file_name,
	                const char* function_name) throw();

	/**
	 * @return A string in the following format:
	 * `<file_name>:<line>: <function_name>`
	 */
	std::string format() const;

	uint_least32_t line() const throw();
	const char* file_name() const throw();
	const char* function_name() const throw();

private:
	uint_least32_t _line;
	const char* _file_name;
	const char* _function_name;
};

} // namespace ft

#endif // LIBFTPP_SOURCE_LOCATION_HPP
