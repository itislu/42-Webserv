#pragma once
#ifndef HTTP_HPP
#define HTTP_HPP

#include <map>
#include <string>

namespace http {
extern const char* const CRLF;

int isSchemeChar(int chr);
int isAuthChar(int chr);
int isReserved(int chr);
int isUnreserved(int chr);
int isGenDelim(int chr);
int isSubDelim(int chr);
int isPchar(int chr);
int isTchar(int chr);
int isHexDigit(int chr);
int isQueryChar(int chr);
int isFragmentChar(int chr);
int isDigit19(int chr);
int isDigit04(int chr);
int isDigit05(int chr);
int isWhitespace(int chr);
int isObsText(int chr);
int isVchar(int chr);

typedef std::map<std::string, std::string> ExtToTypeMap;
const ExtToTypeMap& getExtToType();

}

#endif
