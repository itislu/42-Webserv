#pragma once
#ifndef LIBFTPP_RANDOM_HPP
#	define LIBFTPP_RANDOM_HPP

#	include <ios>

namespace ft {

template <typename T>
void urandom(T* v, std::streamsize size);

template <typename T>
T urandom();

} // namespace ft

#	include "libftpp/random/urandom.tpp" // IWYU pragma: export

#endif // LIBFTPP_RANDOM_HPP
