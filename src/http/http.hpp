#pragma once
#ifndef HTTP_HPP
#define HTTP_HPP

namespace http {
extern const char* const CRLF;

bool isSchemeChar(char chr);
bool isAuthChar(char chr);
bool isReserved(char chr);
bool isUnreserved(char chr);
bool isGenDelim(char chr);
bool isSubDelim(char chr);
bool isPchar(char chr);
bool isTchar(char chr);
bool isQueryChar(char chr);
bool isFragmentChar(char chr);
bool isDigit19(char chr);
bool isDigit04(char chr);
bool isDigit05(char chr);
bool isObsText(char chr);
bool isVchar(char chr);
}

#endif
