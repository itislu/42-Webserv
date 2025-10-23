#pragma once
#ifndef HTTP_HPP
#define HTTP_HPP

// NOLINTBEGIN(readability-identifier-length)
namespace http {
const char SP = ' ';
const char CR = '\r';
const char LF = '\n';
const char CRLF[] = "\r\n";

const char genDelims[] = ":/?#[]@";
const char subDelims[] = "!$&'()*+,;=";
const char specialSchemeChars[] = "+-.";
const char specialAuthorityChars[] = "-._~"
                                     "!$&'()*+,;=:@"
                                     "[]";
const char specialUnreservedChars[] = "-._~";
const char specialTokenChars[] = "!#$%&'*+-.^_`|~";
const char hexDigitsLower[] = "0123456789abcdef";
const char hexDigitsUpper[] = "0123456789ABCDEF";

int isSchemeChar(int chr);
int isAuthChar(int chr);
int isReserved(int chr);
int isUnreserved(int chr);
int isGenDelim(int chr);
int isSubDelim(int chr);
int isPchar(int chr);
int isTchar(int chr);
int isHexDigit(int chr);
int isFragmentChar(int chr);
int isDigit19(int chr);
int isDigit04(int chr);
int isDigit05(int chr);

}
// NOLINTEND(readability-identifier-length)

#endif // HTTP_HPP
