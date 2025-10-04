// IWYU pragma: private; include "libftpp/numeric.hpp"
#pragma once
#ifndef LIBFTPP_NUMERIC_NUMERIC_CAST_TPP
#	define LIBFTPP_NUMERIC_NUMERIC_CAST_TPP

#	include "libftpp/expected.hpp"
#	include "libftpp/numeric.hpp"
#	include "libftpp/type_traits.hpp"
#	include <cmath>
#	include <ios>
#	include <limits>
#	include <new>
#	include <sstream>

namespace ft {

namespace _numeric_cast {
template <typename To, typename From>
To int_to_int(From from);
template <typename To, typename From>
To fp_to_int(From from);
template <typename To, typename From>
To fp_to_fp(From from);
template <typename To, typename From>
To int_to_fp(From from);
template <typename To, typename From>
struct impl {
	static To try_cast(From from);
};
} // namespace _numeric_cast

template <typename To, typename From>
To numeric_cast(From from)
{
	if (ft::is_integral<To>::value) {
		if (ft::is_integral<From>::value) {
			return _numeric_cast::int_to_int<To>(from);
		}
		return _numeric_cast::fp_to_int<To>(from);
	}
	if (ft::is_integral<From>::value) {
		return _numeric_cast::int_to_fp<To>(from);
	}
	return _numeric_cast::fp_to_fp<To>(from);
}

template <typename To, typename From>
ft::expected<To, ft::numeric_cast_exception>
numeric_cast(From from, std::nothrow_t /*unused*/)
{
	try {
		return ft::numeric_cast<To>(from);
	}
	catch (const numeric_cast_exception& e) {
		return ft::unexpected<numeric_cast_exception>(e);
	}
}

namespace _numeric_cast {

template <typename To, typename From>
To int_to_int(From from)
{
	if (!std::numeric_limits<To>::is_signed && from < 0) {
		throw numeric_cast_negative_overflow_exception();
	}
	return impl<To, From>::try_cast(from);
}

template <typename To, typename From>
To fp_to_int(From from)
{
	if (std::isnan(from)) {
		throw numeric_cast_exception();
	}
	if (!std::numeric_limits<To>::is_signed && from < 0) {
		throw numeric_cast_negative_overflow_exception();
	}
	return impl<To, From>::try_cast(from);
}

template <typename To, typename From>
To fp_to_fp(From from)
{
	if (std::isinf(from) || std::isnan(from)) {
		return static_cast<To>(from);
	}
	return impl<To, From>::try_cast(from);
}

template <typename To, typename From>
To int_to_fp(From from)
{
	return impl<To, From>::try_cast(from);
}

template <typename To, typename From>
To impl<To, From>::try_cast(From from)
{
	To to;
	std::stringstream ss;
	ss << std::fixed << from;
	if (!(ss >> to)) {
		from < 0 ? throw numeric_cast_negative_overflow_exception()
		         : throw numeric_cast_positive_overflow_exception();
	}
	return static_cast<To>(from);
}

/**
 * Specializations for char and unsigned char because stringstream doesn't
 * interpret char as a number for insertion, and for extraction just takes the
 * first char literally.
 */

template <typename From>
struct impl<char, From> {
	static char try_cast(From from)
	{
		int to; // NOLINT(cppcoreguidelines-init-variables)
		std::stringstream ss;
		ss << std::fixed << from;
		if (!(ss >> to)) {
			from < 0 ? throw numeric_cast_negative_overflow_exception()
			         : throw numeric_cast_positive_overflow_exception();
		}
		if (to > std::numeric_limits<char>::max()) {
			throw numeric_cast_positive_overflow_exception();
		}
		if (to < std::numeric_limits<char>::min()) {
			throw numeric_cast_negative_overflow_exception();
		}
		return static_cast<char>(from);
	}
};

template <typename From>
struct impl<unsigned char, From> {
	static unsigned char try_cast(From from)
	{
		unsigned int to; // NOLINT(cppcoreguidelines-init-variables)
		std::stringstream ss;
		ss << std::fixed << from;
		if (!(ss >> to)) {
			from < 0 ? throw numeric_cast_negative_overflow_exception()
			         : throw numeric_cast_positive_overflow_exception();
		}
		if (to > std::numeric_limits<unsigned char>::max()) {
			throw numeric_cast_positive_overflow_exception();
		}
		return static_cast<unsigned char>(from);
	}
};

template <typename To>
struct impl<To, char> {
	static To try_cast(char from)
	{
		To to;
		std::stringstream ss;
		ss << static_cast<int>(from);
		if (!(ss >> to)) {
			from < 0 ? throw numeric_cast_negative_overflow_exception()
			         : throw numeric_cast_positive_overflow_exception();
		}
		return static_cast<To>(from);
	}
};

template <typename To>
struct impl<To, unsigned char> {
	static To try_cast(unsigned char from)
	{
		To to;
		std::stringstream ss;
		ss << static_cast<unsigned int>(from);
		if (!(ss >> to)) {
			throw numeric_cast_positive_overflow_exception();
		}
		return static_cast<To>(from);
	}
};

} // namespace _numeric_cast

} // namespace ft

#endif // LIBFTPP_NUMERIC_NUMERIC_CAST_TPP
