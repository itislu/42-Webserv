#include "headerUtils.hpp"

#include <http/Headers.hpp>
#include <http/http.hpp>
#include <libftpp/string.hpp>
#include <utils/fileUtils.hpp>

#include <cstddef>
#include <string>

namespace header {

const char* const host = "Host";
const char* const contentLength = "Content-Length";
const char* const transferEncoding = "Transfer-Encoding";
const char* const contentType = "Content-Type";

} // namespace header

void setContentLengthHeader(Headers& headers, std::size_t length)
{
  const std::string sizeStr = ft::to_string(length);
  headers.setHeader(header::contentLength, sizeStr);
}

void setContentTypeHeader(Headers& headers, const std::string& filePath)
{
  const std::string fileExt = getFileExtension(filePath);
  const http::ExtToTypeMap& extToType = http::getExtToType();
  const http::ExtToTypeMap::const_iterator type = extToType.find(fileExt);

  if (type != extToType.end()) {
    headers.setHeader(header::contentType, type->second);
  }
}
