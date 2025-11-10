#include "config/ConfigTypes.hpp"
#include "config/Converters.hpp"
#include "config/DirectiveHandler.hpp"
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

template<typename ConfigType>
void DirectiveHandler<ConfigType>::checkDirectiveHandler(
  const std::string& key,
  const std::vector<std::string>& values,
  ConfigType& config)
{
  for (std::size_t i = 0; Entries<ConfigType>::entries[i].key != 0; ++i) {
    if (std::strcmp(Entries<ConfigType>::entries[i].key, key.c_str()) == 0) {
      Entries<ConfigType>::entries[i].func(values, config);
      return;
    }
  }
  throw std::invalid_argument("Invalid directive: " + key);
}

template<typename ConfigType>
void DirectiveHandler<ConfigType>::buildDirectives(
  const DirectiveMap& directives,
  ConfigType& config)
{
  for (DirectiveMap::const_iterator it = directives.begin();
       it != directives.end();
       ++it) {
    const std::string& key = it->first;
    const std::vector<std::string>& values = it->second;
    checkDirectiveHandler(key, values, config);
  }
}

// ==================== Setters ====================
template<typename ConfigType>
void DirectiveHandlerBase<ConfigType>::setRoot(
  const std::vector<std::string>& values,
  ConfigType& config)
{
  if (values.size() != 1) {
    throw std::invalid_argument("root: invalid number of arguments");
  }
  config.setRoot(values[0]);
}

template<typename ConfigType>
void DirectiveHandlerBase<ConfigType>::setMaxBodySize(
  const std::vector<std::string>& values,
  ConfigType& config)
{
  if (values.size() != 1) {
    throw std::invalid_argument("max_body_size: invalid number of arguments");
  }
  std::size_t size = toMaxBodySize(values[0]);
  config.setMaxBodySize(size);
}

// error_page 505 504 50x.html 404 404.html
template<typename ConfigType>
void DirectiveHandlerBase<ConfigType>::setErrorPage(
  const std::vector<std::string>& values,
  ConfigType& config)
{
  std::vector<int> codes;
  for (std::size_t i = 0; i < values.size(); ++i) {
    std::stringstream sstream(values[i]);
    int value = 0;
    sstream >> value;

    if (sstream.fail() || !sstream.eof()) {
      config.setErrorPages(codes, values[i]);
      codes.clear();
    } else {
      codes.push_back(value);
    }
  }
}
