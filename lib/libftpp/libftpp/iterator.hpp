#pragma once
#ifndef LIBFTPP_ITERATOR_HPP
#	define LIBFTPP_ITERATOR_HPP

#	include "libftpp/type_traits.hpp"
#	include <cstddef>
#	include <iterator>

namespace ft {

/* Iterator traits */

/**
 * @brief Checks wether `T` is an iterator
 *
 * Provides the member constant `value` which is equal to `true`, if
 * `std::iterator_traits<T>::iterator_category` exists and is one of the
 * following:
 * - `std::input_iterator_tag`
 * - `std::output_iterator_tag`
 * - `std::forward_iterator_tag`
 * - `std::bidirectional_iterator_tag`
 * - `std::random_access_iterator_tag`
 * Otherwise, `value` is equal to `false`.
 *
 * Non-standard.
 */
template <typename T>
struct is_iterator;

/**
 * @brief Checks wether `T` is an input iterator
 *
 * Provides the member constant `value` which is equal to `true`, if
 * `std::iterator_traits<T>::iterator_category` is convertible to
 * `std::input_iterator_tag`. Otherwise, `value` is equal to `false`.
 *
 * Non-standard.
 */
template <typename T>
struct is_input_iterator;

/**
 * @brief Checks wether `T` is an output iterator
 *
 * Provides the member constant `value` which is equal to `true`, if
 * `std::iterator_traits<T>::iterator_category` is convertible to
 * `std::output_iterator_tag`. Otherwise, `value` is equal to `false`.
 *
 * Non-standard.
 */
template <typename T>
struct is_output_iterator;

/**
 * @brief Checks wether `T` is a forward iterator
 *
 * Provides the member constant `value` which is equal to `true`, if
 * `std::iterator_traits<T>::iterator_category` is convertible to
 * `std::forward_iterator_tag`. Otherwise, `value` is equal to `false`.
 *
 * Non-standard.
 */
template <typename T>
struct is_forward_iterator;

/**
 * @brief Checks wether `T` is a bidirectional iterator
 *
 * Provides the member constant `value` which is equal to `true`, if
 * `std::iterator_traits<T>::iterator_category` is convertible to
 * `std::bidirectional_iterator_tag`. Otherwise, `value` is equal to `false`.
 *
 * Non-standard.
 */
template <typename T>
struct is_bidirectional_iterator;

/**
 * @brief Checks wether `T` is a random access iterator
 *
 * Provides the member constant `value` which is equal to `true`, if
 * `std::iterator_traits<T>::iterator_category` is convertible to
 * `std::random_access_iterator_tag`. Otherwise, `value` is equal to `false`.
 *
 * Non-standard.
 */
template <typename T>
struct is_random_access_iterator;

/* Iterator operations */

/**
 * @brief Advances an iterator by a given distance or to a given bound
 *
 * Adds C++20's overload set for `std::advance` from the ranges library which
 * supports bounds as arguments.
 *
 * @note Unlike the standard implementation:
 * - If `it` is a random-access iterator, it is always assumed to be able to
 *   take the difference to `bound`.
 * - `bound` is never used for assignment.
 *
 * https://en.cppreference.com/w/cpp/iterator/ranges/advance
 */
template <typename InputIt>
void advance(InputIt& it,
             typename std::iterator_traits<InputIt>::difference_type n);
template <typename InputIt, typename Sentinel>
FT_REQUIRES((!ft::is_convertible<
             Sentinel,
             typename std::iterator_traits<InputIt>::difference_type>::value))
(void)advance(InputIt& it, Sentinel bound);
template <typename InputIt, typename Sentinel>
typename std::iterator_traits<InputIt>::difference_type
advance(InputIt& it,
        typename std::iterator_traits<InputIt>::difference_type n,
        Sentinel bound);

/**
 * @brief Increment an iterator by a given distance or to a given bound
 *
 * Adds C++20's overload set for C++11's `std::next` from the ranges library
 * which supports bounds as arguments.
 *
 * @note Unlike the standard implementation:
 * - If `it` is a random-access iterator, it is always assumed to be able to
 *   take the difference to `bound`.
 * - `bound` is never used for assignment.
 *
 * https://en.cppreference.com/w/cpp/iterator/ranges/next
 */
template <typename InputIt>
InputIt next(InputIt it);
template <typename InputIt>
InputIt next(InputIt it,
             typename std::iterator_traits<InputIt>::difference_type n);
template <typename InputIt, typename Sentinel>
FT_REQUIRES((!ft::is_convertible<
             Sentinel,
             typename std::iterator_traits<InputIt>::difference_type>::value))
(InputIt) next(InputIt it, Sentinel bound);
template <typename InputIt, typename Sentinel>
InputIt next(InputIt it,
             typename std::iterator_traits<InputIt>::difference_type n,
             Sentinel bound);

/**
 * @brief Decrement an iterator by a given distance or to a given bound
 *
 * Adds C++20's overload set for C++11's `std::prev` from the ranges library
 * which supports bounds as arguments.
 *
 * @note Unlike the standard implementation:
 * - If `it` is a random-access iterator, it is always assumed to be able to
 *   take the difference to `bound`.
 * - `bound` is never used for assignment.
 *
 * https://en.cppreference.com/w/cpp/iterator/ranges/prev
 */
template <typename BidirIt>
BidirIt prev(BidirIt it);
template <typename BidirIt>
BidirIt prev(BidirIt it,
             typename std::iterator_traits<BidirIt>::difference_type n);
template <typename BidirIt>
BidirIt prev(BidirIt it,
             typename std::iterator_traits<BidirIt>::difference_type n,
             BidirIt bound);

/* Range access */

/**
 * https://en.cppreference.com/w/cpp/iterator/begin
 */
template <typename C>
typename C::iterator begin(C& c);
template <typename C>
typename C::const_iterator begin(const C& c);
template <typename T, std::size_t N>
T* begin(T (&array)[N]);
template <typename C>
typename C::const_iterator cbegin(const C& c);
template <typename T, std::size_t N>
const T* cbegin(const T (&array)[N]);

/**
 * https://en.cppreference.com/w/cpp/iterator/end
 */
template <typename C>
typename C::iterator end(C& c);
template <typename C>
typename C::const_iterator end(const C& c);
template <typename T, std::size_t N>
T* end(T (&array)[N]);
template <typename C>
typename C::const_iterator cend(const C& c);
template <typename T, std::size_t N>
const T* cend(const T (&array)[N]);

/**
 * https://en.cppreference.com/w/cpp/iterator/rbegin
 */
template <typename C>
typename C::reverse_iterator rbegin(C& c);
template <typename C>
typename C::const_reverse_iterator rbegin(const C& c);
template <typename T, std::size_t N>
std::reverse_iterator<T*> rbegin(T (&array)[N]);
template <typename C>
typename C::const_reverse_iterator crbegin(const C& c);
template <typename T, std::size_t N>
std::reverse_iterator<const T*> crbegin(const T (&array)[N]);

/**
 * https://en.cppreference.com/w/cpp/iterator/rend
 */
template <typename C>
typename C::reverse_iterator rend(C& c);
template <typename C>
typename C::const_reverse_iterator rend(const C& c);
template <typename T, std::size_t N>
std::reverse_iterator<T*> rend(T (&array)[N]);
template <typename C>
typename C::const_reverse_iterator crend(const C& c);
template <typename T, std::size_t N>
std::reverse_iterator<const T*> crend(const T (&array)[N]);

/**
 * https://en.cppreference.com/w/cpp/iterator/size
 */
template <typename C>
typename C::size_type size(const C& c);
template <typename C>
typename C::difference_type ssize(const C& c);
template <typename T, std::size_t N>
std::size_t size(const T (&array)[N]) throw();
template <typename T, std::ptrdiff_t N>
std::ptrdiff_t ssize(const T (&array)[N]) throw();

/**
 * https://en.cppreference.com/w/cpp/iterator/empty
 */
template <typename C>
bool empty(const C& c);
template <typename T, std::size_t N>
bool empty(const T (&array)[N]) throw();

/**
 * https://en.cppreference.com/w/cpp/iterator/data
 */
template <typename C>
typename C::pointer data(C& c);
template <typename C>
typename C::const_pointer data(const C& c);
template <typename T, std::size_t N>
T* data(T (&array)[N]) throw();

} // namespace ft

#	include "libftpp/iterator/advance.tpp"         // IWYU pragma: export
#	include "libftpp/iterator/begin.tpp"           // IWYU pragma: export
#	include "libftpp/iterator/data.tpp"            // IWYU pragma: export
#	include "libftpp/iterator/empty.tpp"           // IWYU pragma: export
#	include "libftpp/iterator/end.tpp"             // IWYU pragma: export
#	include "libftpp/iterator/iterator_traits.tpp" // IWYU pragma: export
#	include "libftpp/iterator/next.tpp"            // IWYU pragma: export
#	include "libftpp/iterator/prev.tpp"            // IWYU pragma: export
#	include "libftpp/iterator/rbegin.tpp"          // IWYU pragma: export
#	include "libftpp/iterator/rend.tpp"            // IWYU pragma: export
#	include "libftpp/iterator/size.tpp"            // IWYU pragma: export

#endif // LIBFTPP_ITERATOR_HPP
