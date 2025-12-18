#pragma once
#ifndef HTTP_HPP
#define HTTP_HPP

#include <map>
#include <netinet/in.h>
#include <string>

namespace http {

extern const char* const CRLF;
extern const char* const HTTP_1_0;
extern const char* const HTTP_1_1;
extern const char* const HTTP_2_0;
extern const in_port_t httpPort;

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
bool isQdTextChar(char chr);
bool isQuotedPairChar(char chr);

typedef std::map<std::string, std::string> ExtToTypeMap;
const ExtToTypeMap& getExtToType();

extern const char* const minResponse408;
extern const char* const minResponse500;
extern const char* const minResponse504;

} // namespace http

#endif
