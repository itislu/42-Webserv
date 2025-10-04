#pragma once
#ifndef LIBFTPP_ALGORITHM_HPP
#	define LIBFTPP_ALGORITHM_HPP

#	include <cstddef>
#	include <iterator>
#	include <utility>

#	define FT_MIN(a, b) ((b) < (a) ? (b) : (a))
#	define FT_MAX(a, b) ((b) > (a) ? (b) : (a))

namespace ft {

/**
 * https://en.cppreference.com/w/cpp/algorithm/copy
 */
template <typename InputIt, typename OutputIt, typename UnaryPred>
OutputIt copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPred pred);

/**
 * https://en.cppreference.com/w/cpp/algorithm/copy_n
 */
template <typename InputIt, typename Size, typename OutputIt>
OutputIt copy_n(InputIt first, Size count, OutputIt result);

/**
 * @brief Determines if two sets of elements are the same
 *
 * Adds C++14's overload set for `std::equal` which supports passing an end for
 * the second range as argument, instead of using
 * `std::distance(first1, last1)`.
 *
 * https://en.cppreference.com/w/cpp/algorithm/equal
 */
template <typename InputIt1, typename InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2);
template <typename InputIt1, typename InputIt2, typename BinaryPred>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPred p);
template <typename InputIt1, typename InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2);
template <typename InputIt1, typename InputIt2, typename BinaryPred>
bool equal(InputIt1 first1,
           InputIt1 last1,
           InputIt2 first2,
           InputIt2 last2,
           BinaryPred p);

/**
 * https://en.cppreference.com/w/cpp/algorithm/is_sorted
 */
template <typename ForwardIt>
bool is_sorted(ForwardIt first, ForwardIt last);
template <typename ForwardIt, typename Compare>
bool is_sorted(ForwardIt first, ForwardIt last, Compare comp);

/**
 * https://en.cppreference.com/w/cpp/algorithm/is_sorted_until
 */
template <typename ForwardIt>
ForwardIt is_sorted_until(ForwardIt first, ForwardIt last);
template <typename ForwardIt, typename Compare>
ForwardIt is_sorted_until(ForwardIt first, ForwardIt last, Compare comp);

/**
 * @brief Optimized version of `std::lower_bound` for non-random-access
 * iterators
 *
 * If the size of the range to search is already known, passing it to this
 * functions avoids one full iteration over the range, reducing the worst-case
 * iterations to exactly N.
 *
 * https://en.cppreference.com/w/cpp/algorithm/lower_bound
 */
template <typename ForwardIt, typename T>
ForwardIt
lower_bound(ForwardIt first,
            typename std::iterator_traits<ForwardIt>::difference_type count,
            const T& value);
template <typename ForwardIt, typename T, typename Compare>
ForwardIt
lower_bound(ForwardIt first,
            typename std::iterator_traits<ForwardIt>::difference_type count,
            const T& value,
            Compare comp);

/**
 * @brief Optimized version of `std::upper_bound` for non-random-access
 * iterators
 *
 * If the size of the range to search is already known, passing it to this
 * functions avoids one full iteration over the range, reducing the worst-case
 * iterations to exactly N.
 *
 * https://en.cppreference.com/w/cpp/algorithm/upper_bound
 */
template <typename ForwardIt, typename T>
ForwardIt
upper_bound(ForwardIt first,
            typename std::iterator_traits<ForwardIt>::difference_type count,
            const T& value);
template <typename ForwardIt, typename T, typename Compare>
ForwardIt
upper_bound(ForwardIt first,
            typename std::iterator_traits<ForwardIt>::difference_type count,
            const T& value,
            Compare comp);

/**
 * @brief Optimized version of `std::equal_range` for non-random-access
 * iterators
 *
 * If the size of the range to search is already known, passing it to this
 * functions avoids one full iteration over the range, reducing the worst-case
 * iterations to exactly N.
 *
 * https://en.cppreference.com/w/cpp/algorithm/equal_range
 */
template <typename ForwardIt, typename T>
std::pair<ForwardIt, ForwardIt>
equal_range(ForwardIt first,
            typename std::iterator_traits<ForwardIt>::difference_type count,
            const T& value);
template <typename ForwardIt, typename T, typename Compare>
std::pair<ForwardIt, ForwardIt>
equal_range(ForwardIt first,
            typename std::iterator_traits<ForwardIt>::difference_type count,
            const T& value,
            Compare comp);

/**
 * @brief Optimized version of `std::binary_search` for non-random-access
 * iterators
 *
 * If the size of the range to search is already known, passing it to this
 * functions avoids one full iteration over the range, reducing the worst-case
 * iterations to exactly N.
 *
 * https://en.cppreference.com/w/cpp/algorithm/binary_search
 */
template <typename ForwardIt, typename T>
bool
binary_search(ForwardIt first,
              typename std::iterator_traits<ForwardIt>::difference_type count,
              const T& value);
template <typename ForwardIt, typename T, typename Compare>
bool
binary_search(ForwardIt first,
              typename std::iterator_traits<ForwardIt>::difference_type count,
              const T& value,
              Compare comp);

/**
 * @brief Non-standard version of `std::max` which allows assigning to the
 * result.
 */
template <typename T>
T& max(T& a, T& b);
template <typename T, typename Compare>
T& max(T& a, T& b, Compare comp);
/**
 * https://en.cppreference.com/w/cpp/algorithm/max
 */
template <typename T>
const T& max(const T& a, const T& b);
template <typename T, typename Compare>
const T& max(const T& a, const T& b, Compare comp);

/**
 * @brief Non-standard version of `std::min` which allows assigning to the
 * result.
 */
template <typename T>
T& min(T& a, T& b);
template <typename T, typename Compare>
T& min(T& a, T& b, Compare comp);
/**
 * https://en.cppreference.com/w/cpp/algorithm/min
 */
template <typename T>
const T& min(const T& a, const T& b);
template <typename T, typename Compare>
const T& min(const T& a, const T& b, Compare comp);

/**
 * https://en.cppreference.com/w/cpp/algorithm/shift
 */
template <typename ForwardIt>
ForwardIt
shift_left(ForwardIt first,
           ForwardIt last,
           typename std::iterator_traits<ForwardIt>::difference_type n);

/**
 * https://en.cppreference.com/w/cpp/algorithm/shift
 */
template <typename ForwardIt>
ForwardIt
shift_right(ForwardIt first,
            ForwardIt last,
            typename std::iterator_traits<ForwardIt>::difference_type n);

/**
 * @brief Generic swap implementation which uses the type's internal `swap`
 * function, if possible, otherwise falls back to the common
 * `using std::swap; swap(a, b);` pattern.
 *
 * This can be useful when a type only has a member `swap` function and no
 * non-member `swap` can be added safely.
 *
 * https://en.cppreference.com/w/cpp/algorithm/swap
 * https://en.cppreference.com/w/cpp/named_req/Swappable
 */
template <typename T>
void member_swap(T& a, T& b);

/**
 * https://en.cppreference.com/w/cpp/algorithm/swap
 */
template <typename T2, std::size_t N>
void member_swap(T2 (&a)[N], T2 (&b)[N]);

/**
 * @brief Uses `ft::member_swap`
 *
 * https://en.cppreference.com/w/cpp/algorithm/iter_swap
 */
template <typename ForwardIt1, typename ForwardIt2>
void iter_swap(ForwardIt1 a, ForwardIt2 b);

/**
 * @brief Uses `ft::member_swap`
 *
 * https://en.cppreference.com/w/cpp/algorithm/swap_ranges
 */
template <typename ForwardIt1, typename ForwardIt2>
ForwardIt2 swap_ranges(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2);

} // namespace ft

#	include "libftpp/algorithm/binary_search.tpp" // IWYU pragma: export
#	include "libftpp/algorithm/copy.tpp"          // IWYU pragma: export
#	include "libftpp/algorithm/equal.tpp"         // IWYU pragma: export
#	include "libftpp/algorithm/is_sorted.tpp"     // IWYU pragma: export
#	include "libftpp/algorithm/min_max.tpp"       // IWYU pragma: export
#	include "libftpp/algorithm/shift.tpp"         // IWYU pragma: export
#	include "libftpp/algorithm/swap.tpp"          // IWYU pragma: export

#endif // LIBFTPP_ALGORITHM_HPP
