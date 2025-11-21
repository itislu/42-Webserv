#include "http.hpp"

#include <libftpp/algorithm.hpp>

#include <ctype.h>
#include <map>
#include <string>

namespace http {

const char* const CRLF = "\r\n";

int isSchemeChar(int chr)
{
  if (::isalnum(chr) != 0) {
    return 1;
  }

  static const char specialSchemeChars[] = "+-.";
  return static_cast<int>(
    ft::contains(specialSchemeChars, static_cast<char>(chr)));
}

int isAuthChar(int chr)
{
  if (::isalnum(chr) != 0) {
    return 1;
  }

  static const char specialAuthorityChars[] = "-._~"
                                              "!$&'()*+,;=:@"
                                              "[]";
  return static_cast<int>(
    ft::contains(specialAuthorityChars, static_cast<char>(chr)));
}

int isReserved(int chr)
{
  return static_cast<int>(isGenDelim(chr) != 0 || isSubDelim(chr) != 0);
}

/**
 * unreserved  = ALPHA / DIGIT / "-" / "." / "_" / "~"
 */
int isUnreserved(int chr)
{
  if (::isalnum(chr) != 0) {
    return 1;
  }

  static const char specialUnreservedChars[] = "-._~";
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
  static const char genDelims[] = ":/?#[]@";
  return static_cast<int>(ft::contains(genDelims, static_cast<char>(chr)));
}

/**
 * sub-delims = "!" / "$" / "&" / "'" / "(" / ")" /
 *              "*" / "+" / "," / ";" / "="
 */
int isSubDelim(int chr)
{
  static const char subDelims[] = "!$&'()*+,;=";
  return static_cast<int>(ft::contains(subDelims, static_cast<char>(chr)));
}

/**
 * pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
 */
int isPchar(int chr)
{
  return static_cast<int>((isUnreserved(chr) != 0) || (isSubDelim(chr) != 0) ||
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

  static const char specialTokenChars[] = "!#$%&'*+-.^_`|~";
  return static_cast<int>(
    ft::contains(specialTokenChars, static_cast<char>(chr)));
}

int isHexDigit(int chr)
{
  static const char hexDigitsLower[] = "0123456789abcdef";
  static const char hexDigitsUpper[] = "0123456789ABCDEF";

  if (ft::contains(hexDigitsLower, static_cast<char>(chr))) {
    return 1;
  }
  if (ft::contains(hexDigitsUpper, static_cast<char>(chr))) {
    return 1;
  }
  return 0;
}

/**
 * https://datatracker.ietf.org/doc/html/rfc3986#autoid-69
 *
 * query = *( pchar / "/" / "?" )
 */
int isQueryChar(int chr)
{
  return static_cast<int>(chr == '/' || chr == '?');
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

/**
 * ( SP / HTAB )
 */
int isWhitespace(int chr)
{
  return static_cast<int>(chr == ' ' || chr == '\t');
}

/**
 * https://datatracker.ietf.org/doc/html/rfc9110/#name-field-values
 *
 * obs-text = %x80-FF
 */
int isObsText(int chr)
{
  const int begin = 0x80;
  const int end = 0xFF;
  return static_cast<int>(chr >= begin && chr <= end);
}

/**
 * https://datatracker.ietf.org/doc/html/rfc5234#autoid-25
 *
 * VCHAR =  %x21-7E
 *          ; visible (printing) characters
 */
int isVchar(int chr)
{
  const char begin = 0x21;
  const char end = 0x7E;
  return static_cast<int>(chr >= begin && chr <= end);
}

const ExtToTypeMap& getExtToType()
{
  static std::map<std::string, std::string> map;
  if (map.empty()) {
    map[".html"] = "text/html";
    map[".css"] = "text/css";
    map[".js"] = "application/javascript";
    map[".txt"] = "text/plain";
    map[".png"] = "image/png";
    map[".jpg"] = "image/jpeg";
    map[".jpeg"] = "image/jpeg";
    map[".gif"] = "image/gif";
    map[".ico"] = "image/x-icon";
    map[".pdf"] = "application/pdf";
    map[".json"] = "application/json";
  }
  return map;
}
}
