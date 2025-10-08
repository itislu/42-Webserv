#pragma once
#ifndef LIBFTPP_STRING_HPP
#	define LIBFTPP_STRING_HPP

#	include "libftpp/exception.hpp"
#	include "libftpp/expected.hpp"
#	include <cstddef>
#	include <ios>
#	include <new>
#	include <string>
#	include <typeinfo>

namespace ft {

/* starts_with */

/**
 * https://en.cppreference.com/w/cpp/string/basic_string/starts_with
 */
template <typename CharT, typename Traits, typename Allocator>
bool starts_with(const std::basic_string<CharT, Traits, Allocator>& str,
                 const std::basic_string<CharT, Traits, Allocator>& prefix);
template <typename CharT, typename Traits, typename Allocator>
bool starts_with(const std::basic_string<CharT, Traits, Allocator>& str,
                 CharT ch);
template <typename CharT, typename Traits, typename Allocator>
bool starts_with(const std::basic_string<CharT, Traits, Allocator>& str,
                 const CharT* prefix);

/* ends_with */

/**
 * https://en.cppreference.com/w/cpp/string/basic_string/ends_with
 */
template <typename CharT, typename Traits, typename Allocator>
bool ends_with(const std::basic_string<CharT, Traits, Allocator>& str,
               const std::basic_string<CharT, Traits, Allocator>& suffix);
template <typename CharT, typename Traits, typename Allocator>
bool ends_with(const std::basic_string<CharT, Traits, Allocator>& str,
               CharT ch);
template <typename CharT, typename Traits, typename Allocator>
bool ends_with(const std::basic_string<CharT, Traits, Allocator>& str,
               const CharT* suffix);

/* from_string */

class from_string_exception;
class from_string_range_exception;
class from_string_invalid_exception;

/**
 * @brief Parses a string and converts it to a value of the specified type `To`
 *
 * The behavior is mostly the same as `std::istream`, except:
 * - For unsigned integral types, negative values do not wrap around and are
 *   considered out of range (`-0` is within range).
 * - For floating point types, infinity and NaN can be parsed.
 * - For floating point types, if `std::ios::fixed` is in `fmt` without
 *   `std::ios::scientific`, scientific notation is considered invalid.
 *
 * Default rules (when `fmt` is not provided):
 * - Leading whitespace is *not* skipped. Use `std::ios::skipws` in `fmt` to
 *   enable skipping.
 * - For integral types, the base is auto-detected from prefixes: `0` for octal,
 *   `0x` or `0X` for hexadecimal. If no prefix is present, base 10 is assumed.
 *   A standalone hex-prefix is considered invalid. The base can be explicitly
 *   set using `std::ios::dec`, `std::ios::oct`, `std::ios::hex` in `fmt`.
 * - For `bool`, accepts alphabetic ("true", "false") and numeric ("1", "0")
 *   representations. `std::ios::boolalpha` in `fmt` restricts parsing to
 *   alphabetic only.
 *
 * Parsing stops at the first invalid character for the expected format, target
 * type or base.
 *
 * Overloads with the `nothrow` parameter return a `ft::expected` and do not
 * throw exceptions on conversion errors.
 *
 * @return For throwing overloads: The converted value of type `To`
 * @return For non-throwing overloads: A `ft::expected<To,
 * ft::from_string_exception>` containing the converted value on success, or a
 * `ft::from_string_exception` on failure
 *
 * @throws ft::from_string_invalid_exception (Throwing overloads only) If the
 * string `str` does not represent a valid value for type `To` according to the
 * specified format (e.g., non-numeric characters for an integer, invalid base
 * prefix, or non-boolean string for `bool`)
 * @throws ft::from_string_range_exception (Throwing overloads only) If the
 * parsed value is valid but falls outside the representable range of type `To`.
 * For `bool` using numeric format, this is thrown if the value is numeric but
 * not 1 or 0
 *
 * @tparam To The target type to convert the string to
 * @param str The input string to parse
 * @param endpos_out (optional) Out-parameter which, if not null, is set to the
 * number of valid characters in `str` before considering range errors, or 0 if
 * the conversion is *invalid*
 */
template <typename To>
To from_string(const std::string& str,
               std::string::size_type* endpos_out = NULL);
/**
 * @copydoc from_string(const std::string&, std::string::size_type*)
 *
 * @param fmt (optional) Stream format flags (`std::ios::fmtflags`) to control
 * parsing behavior (e.g., base, skipping whitespace, scientific notation)
 */
template <typename To>
To from_string(const std::string& str,
               std::ios::fmtflags fmt,
               std::string::size_type* endpos_out = NULL);
/**
 * @copydoc from_string(const std::string&, std::string::size_type*)
 *
 * @param nothrow (optional) A tag (`std::nothrow`) selecting the non-throwing
 * overload
 */
template <typename To>
ft::expected<To, ft::from_string_exception>
from_string(const std::string& str,
            std::nothrow_t nothrow,
            std::string::size_type* endpos_out = NULL);
/**
 * @copydoc from_string(const std::string&, std::string::size_type*)
 *
 * @param fmt (optional) Stream format flags (`std::ios::fmtflags`) to control
 * parsing behavior (e.g., base, skipping whitespace, scientific notation)
 * @param nothrow (optional) A tag (`std::nothrow`) selecting the non-throwing
 * overload
 */
template <typename To>
ft::expected<To, ft::from_string_exception>
from_string(const std::string& str,
            std::ios::fmtflags fmt,
            std::nothrow_t nothrow,
            std::string::size_type* endpos_out = NULL);

class from_string_exception : public ft::exception {
public:
	from_string_exception(const std::string& msg,
	                      const std::string& input,
	                      const std::type_info& type_id);
	from_string_exception(const from_string_exception& other) throw();
	~from_string_exception() throw();
	from_string_exception& operator=(from_string_exception other) throw();

	const std::string& input() const throw();
	const std::type_info& type_id() const throw();
	void swap(from_string_exception& other) throw();

private:
	from_string_exception();

	std::string _input;
	const std::type_info* _type_id;
};

class from_string_range_exception : public from_string_exception {
public:
	from_string_range_exception(const std::string& input,
	                            const std::type_info& type_id);
};

class from_string_invalid_exception : public from_string_exception {
public:
	from_string_invalid_exception(const std::string& input,
	                              const std::type_info& type_id);
};

// NOLINTBEGIN(misc-non-private-member-variables-in-classes)

// Functor
template <typename T>
struct from_string_fn {
	T operator()(const std::string& str);
};

// Functor
template <typename T>
struct from_string_fmt_fn {
	explicit from_string_fmt_fn(std::ios::fmtflags fmt_);

	T operator()(const std::string& str);

	std::ios::fmtflags fmt;
};

// NOLINTEND(misc-non-private-member-variables-in-classes)

/* to_string */

/**
 * https://en.cppreference.com/w/cpp/string/basic_string/to_string
 */
template <typename T>
std::string to_string(T v, std::ios::fmtflags fmt = std::ios::fmtflags());
std::string to_string(bool v, std::ios::fmtflags fmt = std::ios::boolalpha);

/* trim */

std::string& trim(std::string& str);
std::string trim(const std::string& str);

/* to_upper */

std::string& to_upper(std::string& str);
std::string to_upper(const std::string& str);

/* to_lower */

std::string& to_lower(std::string& str);
std::string to_lower(const std::string& str);

} // namespace ft

#	include "libftpp/string/ends_with.tpp"      // IWYU pragma: export
#	include "libftpp/string/from_string.tpp"    // IWYU pragma: export
#	include "libftpp/string/from_string_fn.tpp" // IWYU pragma: export
#	include "libftpp/string/starts_with.tpp"    // IWYU pragma: export
#	include "libftpp/string/to_string.tpp"      // IWYU pragma: export

#endif // LIBFTPP_STRING_HPP
