#pragma once
#ifndef HTTP_HPP
#define HTTP_HPP

// NOLINTBEGIN(readability-identifier-length,
// clang-diagnostic-unused-const-variable)
namespace http {
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
// NOLINTEND(readability-identifier-length,
// clang-diagnostic-unused-const-variable)

#endif // HTTP_HPP
