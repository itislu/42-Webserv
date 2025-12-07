#pragma once
#ifndef HEADER_UTILS_HPP
#define HEADER_UTILS_HPP

#include <http/Headers.hpp>

#include <cstddef>
#include <string>
#include <vector>

namespace header {
extern const char* const host;
extern const char* const contentLength;
extern const char* const transferEncoding;
extern const char* const contentType;
}

void setContentLengthHeader(Headers& headers, std::size_t length);
void setContentTypeHeader(Headers& headers, const std::string& filePath);
std::vector<std::string> convertHeaderList(const std::string& rawList);

#endif
