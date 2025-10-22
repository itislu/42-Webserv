#include "http.hpp"

#include <libftpp/algorithm.hpp>

#include <cctype>

namespace http {

int isSchemeChar(int chr)
{
  if (::isalnum(chr) != 0) {
    return 1;
  }
  return static_cast<int>(ft::contains(specialSchemeChars, chr));
}

int isAuthChar(int chr)
{
  if (::isalnum(chr) != 0) {
    return 1;
  }
  return static_cast<int>(ft::contains(specialAuthorityChars, chr));
}

int isReserved(int chr)
{
  return static_cast<int>(isGenDelim(chr) != 0 || isSubdelim(chr) != 0);
}

/**
 * reserved = gen-delims / sub-delims
 */
int isUnreserved(int chr)
{
  if (::isalnum(chr) != 0) {
    return 1;
  }
  if (ft::contains(specialUnreservedChars, static_cast<char>(chr))) {
    return 1;
  }
  return 0;
}

/**
 * gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"
 */
int isGenDelim(int chr)
{
  return static_cast<int>(ft::contains(genDelims, static_cast<char>(chr)));
}

/**
 * sub-delims = "!" / "$" / "&" / "'" / "(" / ")" /
 *              "*" / "+" / "," / ";" / "="
 */
int isSubdelim(int chr)
{
  return static_cast<int>(ft::contains(subDelims, static_cast<char>(chr)));
}

/**
 * pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
 */
int isPchar(int chr)
{
  return static_cast<int>((isUnreserved(chr) != 0) || (isSubdelim(chr) != 0) ||
                          chr == ':' || chr == '@');
}

/**
 * tchar = "!" / "#" / "$" / "%" / "&" / "'" / "*"
 *         / "+" / "-" / "." / "^" / "_" / "`" / "|" / "~"
 *         / DIGIT / ALPHA
 *         ; any VCHAR, except delimiters
 */
int isTchar(int chr)
{
  if (::isalnum(chr) != 0) {
    return 1;
  }
  return static_cast<int>(
    ft::contains(specialTokenChars, static_cast<char>(chr)));
}

int isHexDigit(int chr)
{
  if (ft::contains(hexDigitsLower, chr)) {
    return 1;
  }
  if (ft::contains(hexDigitsUpper, chr)) {
    return 1;
  }
  return 0;
}

/**
 * https://datatracker.ietf.org/doc/html/rfc3986#autoid-69
 *
 * fragment = *( pchar / "/" / "?" )
 */
int isFragmentChar(int chr)
{
  return static_cast<int>(chr == '/' || chr == '?');
}

/**
 * digit 1-9
 */
int isDigit19(int chr)
{
  return static_cast<int>(chr >= '1' && chr <= '9');
}

/**
 * digit 0-4
 */
int isDigit04(int chr)
{
  return static_cast<int>(chr >= '0' && chr <= '4');
}

/**
 * digit 0-5
 */
int isDigit05(int chr)
{
  return static_cast<int>(chr >= '0' && chr <= '5');
}
}
