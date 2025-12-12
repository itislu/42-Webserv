#include "headerUtils.hpp"

#include <http/Headers.hpp>
#include <http/http.hpp>
#include <libftpp/string.hpp>
#include <utils/fileUtils.hpp>

#include <cstddef>
#include <string>
#include <vector>

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

/**
 * @brief Convert raw header list to vector of strings.
 *
 * #element => [ element ] *( OWS "," OWS [ element ] )
 *
 * - removes whitespace
 */
std::vector<std::string> convertHeaderList(const std::string& rawList)
{
  std::vector<std::string> result;

  std::string token;
  for (std::size_t i = 0; i < rawList.size(); ++i) {
    if (rawList[i] == ',') {
      ft::trim(token);
      if (!token.empty()) {
        result.push_back(token);
        token.clear();
      }
    } else {
      token += rawList[i];
    }
  }

  ft::trim(token);
  if (!token.empty()) {
    result.push_back(token);
  }
  return result;
}
