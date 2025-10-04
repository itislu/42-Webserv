#pragma once
#ifndef LIBFTPP_PREPROCESSOR_HPP
#	define LIBFTPP_PREPROCESSOR_HPP

/* Preprocessor Metaprogramming - Macros for manipulating code as text */

/**
 * @brief Append a unique number to `NAME`
 *
 * This macro uses the common predefined `__COUNTER__` macro, which guarantees a
 * new number every time it is used.
 * In the rare case that `__COUNTER__` is not supported, `__LINE__` is used as a
 * fallback, which can lead to duplicate names.
 *
 * Inspiration: https://stackoverflow.com/a/71899854
 */
#	ifdef __COUNTER__
#		define FT_APPEND_UNIQUE_NUM(NAME) FT_CONCAT_EXPANDED(NAME, __COUNTER__)
#	else // __COUNTER__
#		define FT_APPEND_UNIQUE_NUM(NAME) FT_CONCAT_EXPANDED(NAME, __LINE__)
#	endif // __COUNTER__

/**
 * @brief Concatenate `PREFIX` and `SUFFIX` into `PREFIXSUFFIX`
 *
 * Inspiration: https://stackoverflow.com/a/71899854
 */
#	define FT_CONCAT(PREFIX, SUFFIX) PREFIX##SUFFIX

/**
 * @brief Expand `PREFIX` and `SUFFIX` before concatenating
 *
 * Inspiration: https://stackoverflow.com/a/71899854
 */
#	define FT_CONCAT_EXPANDED(PREFIX, SUFFIX) FT_CONCAT(PREFIX, SUFFIX)

#endif // LIBFTPP_PREPROCESSOR_HPP
