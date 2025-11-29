#include "http.hpp"

#include <libftpp/algorithm.hpp>
#include <libftpp/ctype.hpp>

namespace http {

const char* const CRLF = "\r\n";

bool isSchemeChar(char chr)
{
  if (ft::isalnum(chr)) {
    return true;
  }

  static const char specialSchemeChars[] = "+-.";
  return ft::contains(specialSchemeChars, chr);
}

bool isAuthChar(char chr)
{
  if (ft::isalnum(chr)) {
    return true;
  }

  static const char specialAuthorityChars[] = "-._~"
                                              "!$&'()*+,;=:@"
                                              "[]";
  return ft::contains(specialAuthorityChars, chr);
}

bool isReserved(char chr)
{
  return isGenDelim(chr) || isSubDelim(chr);
}

/**
 * unreserved  = ALPHA / DIGIT / "-" / "." / "_" / "~"
 */
bool isUnreserved(char chr)
{
  if (ft::isalnum(chr)) {
    return true;
  }

  static const char specialUnreservedChars[] = "-._~";
  return ft::contains(specialUnreservedChars, chr);
}

/**
 * gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"
 */
bool isGenDelim(char chr)
{
  static const char genDelims[] = ":/?#[]@";
  return ft::contains(genDelims, chr);
}

/**
 * sub-delims = "!" / "$" / "&" / "'" / "(" / ")" /
 *              "*" / "+" / "," / ";" / "="
 */
bool isSubDelim(char chr)
{
  static const char subDelims[] = "!$&'()*+,;=";
  return ft::contains(subDelims, chr);
}

/**
 * pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
 */
bool isPchar(char chr)
{
  return isUnreserved(chr) || isSubDelim(chr) || chr == ':' || chr == '@';
}

/**
 * tchar = "!" / "#" / "$" / "%" / "&" / "'" / "*"
 *         / "+" / "-" / "." / "^" / "_" / "`" / "|" / "~"
 *         / DIGIT / ALPHA
 *         ; any VCHAR, except delimiters
 */
bool isTchar(char chr)
{
  if (ft::isalnum(chr)) {
    return true;
  }

  static const char specialTokenChars[] = "!#$%&'*+-.^_`|~";
  return ft::contains(specialTokenChars, chr);
}

/**
 * https://datatracker.ietf.org/doc/html/rfc3986#autoid-69
 *
 * query = *( pchar / "/" / "?" )
 */
bool isQueryChar(char chr)
{
  return chr == '/' || chr == '?';
}

/**
 * https://datatracker.ietf.org/doc/html/rfc3986#autoid-69
 *
 * fragment = *( pchar / "/" / "?" )
 */
bool isFragmentChar(char chr)
{
  return chr == '/' || chr == '?';
}

/**
 * digit 1-9
 */
bool isDigit19(char chr)
{
  return chr >= '1' && chr <= '9';
}

/**
 * digit 0-4
 */
bool isDigit04(char chr)
{
  return chr >= '0' && chr <= '4';
}

/**
 * digit 0-5
 */
bool isDigit05(char chr)
{
  return chr >= '0' && chr <= '5';
}

/**
 * ( SP / HTAB )
 */
bool isWhitespace(char chr)
{
  return chr == ' ' || chr == '\t';
}

/**
 * https://datatracker.ietf.org/doc/html/rfc9110/#name-field-values
 *
 * obs-text = %x80-FF
 */
bool isObsText(char chr)
{
  const unsigned char begin = 0x80;
  const unsigned char end = 0xFF;
  const unsigned char uchr = static_cast<unsigned char>(chr);
  return uchr >= begin && uchr <= end;
}

/**
 * https://datatracker.ietf.org/doc/html/rfc5234#autoid-25
 *
 * VCHAR =  %x21-7E
 *          ; visible (printing) characters
 */
bool isVchar(char chr)
{
  const char begin = 0x21;
  const char end = 0x7E;
  return chr >= begin && chr <= end;
}

}
