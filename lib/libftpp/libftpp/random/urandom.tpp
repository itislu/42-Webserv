// IWYU pragma: private; include "libftpp/random.hpp"
#pragma once
#ifndef LIBFTPP_RANDOM_URANDOM_TPP
#	define LIBFTPP_RANDOM_URANDOM_TPP

#	include "libftpp/random.hpp"
#	include <fstream>
#	include <ios>

namespace ft {

// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
template <typename T>
void urandom(T* v, std::streamsize size)
{
	std::ifstream urandom;

	urandom.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	urandom.open("/dev/urandom", std::ios::binary);
	urandom.read(reinterpret_cast<char*>(v), size);
	urandom.close();
}
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

template <typename T>
T urandom()
{
	T t;
	ft::urandom(&t, sizeof(t));
	return t;
}

} // namespace ft

#endif // LIBFTPP_RANDOM_URANDOM_TPP
