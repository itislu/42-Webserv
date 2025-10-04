#pragma once

#include <ios>

namespace ft {

template <typename T>
void urandom(T* v, std::streamsize size);

template <typename T>
T urandom();

} // namespace ft

#include "libftpp/random/urandom.tpp" // IWYU pragma: export
