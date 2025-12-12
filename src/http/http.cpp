#include "http.hpp"

#include <libftpp/algorithm.hpp>
#include <libftpp/ctype.hpp>

#include <map>
#include <string>

namespace http {

/**
 * https://datatracker.ietf.org/doc/html/rfc5234#appendix-B.1
 *
 * CRLF           =  CR LF
 *                ; Internet standard newline
 * CR             =  %x0D
 *                ; carriage return
 * LF             =  %x0A
 *                ; linefeed
 */
const char* const CRLF = "\r\n";

const char* const HTTP_1_0 = "HTTP/1.0";
const char* const HTTP_1_1 = "HTTP/1.1";
const char* const HTTP_2_0 = "HTTP/2.0";

/**
 * scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
 */
bool isSchemeChar(char chr)
{
  if (ft::isalnum(chr)) {
    return true;
  }

  static const char specialSchemeChars[] = { '+', '-', '.' };
  return ft::contains(specialSchemeChars, chr);
}

bool isAuthChar(char chr)
{
  if (ft::isalnum(chr)) {
    return true;
  }

  static const char specialAuthorityChars[] = { '-', '.', '_',  '~', '!',
                                                '$', '&', '\'', '(', ')',
                                                '*', '+', ',',  ';', '=',
                                                ':', '@', '[',  ']' };
  return ft::contains(specialAuthorityChars, chr);
}

/**
 * reserved    = gen-delims / sub-delims
 */
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

  static const char specialUnreservedChars[] = { '-', '.', '_', '~' };
  return ft::contains(specialUnreservedChars, chr);
}

/**
 * gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"
 */
bool isGenDelim(char chr)
{
  static const char genDelims[] = { ':', '/', '?', '#', '[', ']', '@' };
  return ft::contains(genDelims, chr);
}

/**
 * sub-delims = "!" / "$" / "&" / "'" / "(" / ")" /
 *              "*" / "+" / "," / ";" / "="
 */
bool isSubDelim(char chr)
{
  static const char subDelims[] = { '!', '$', '&', '\'', '(', ')',
                                    '*', '+', ',', ';',  '=' };
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

  static const char specialTokenChars[] = { '!',  '#', '$', '%', '&',
                                            '\'', '*', '+', '-', '.',
                                            '^',  '_', '`', '|', '~' };
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

/**
 *qdtext         = HTAB / SP / %x21 / %x23-5B / %x5D-7E / obs-text
 */
bool isQdTextChar(char chr)
{
  const char char1 = 0x21;
  const char beg1 = 0x23;
  const char end1 = 0x5B;
  const char beg2 = 0x5D;
  const char end2 = 0x7E;
  return chr == '\t' || chr == ' ' || chr == char1 ||
         (chr >= beg1 && chr <= end1) || (chr >= beg2 && chr <= end2) ||
         isObsText(chr);
}

/**
 * quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )
 */
bool isQuotedPairChar(char chr)
{
  return chr == '\t' || chr == ' ' || isVchar(chr) || isObsText(chr);
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

const char* const minResponse500 = "HTTP/1.1 500 Internal Server Error\r\n"
                                   "Content-Length: 0\r\n"
                                   "Connection: close\r\n\r\n";

} // namespace http
