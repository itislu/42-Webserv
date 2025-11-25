#pragma once
#ifndef LIBFTPP_CTYPE_HPP
#	define LIBFTPP_CTYPE_HPP

/** @file
 * Wrapper functions for standard `ctype` functions that avoid undefined
 * behavior.
 *
 * The behavior of the standard `ctype` functions is undefined if their
 * arguments' values are neither representable as `unsigned char` nor equal to
 * `EOF`.
 * To avoid that, the arguments get cast to `unsigned char` before being passed
 * to the standard functions.
 *
 * The argument and return types are also changed to indicate more clearly what
 * values they support.
 */

namespace ft {

/* Character classification */

bool isalnum(char ch);
bool isalpha(char ch);
bool islower(char ch);
bool isupper(char ch);
bool isdigit(char ch);
bool isxdigit(char ch);
bool iscntrl(char ch);
bool isgraph(char ch);
bool isspace(char ch);
bool isblank(char ch);
bool isprint(char ch);
bool ispunct(char ch);

/* Character manipulation */

char tolower(char ch);
char toupper(char ch);

} // namespace ft

#endif // LIBFTPP_CTYPE_HPP
