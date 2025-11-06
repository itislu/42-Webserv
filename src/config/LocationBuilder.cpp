#include "LocationBuilder.hpp"
#include "config/LocationConfig.hpp"
#include "config/ParsedConfig.hpp"
#include "config/ParsedLocation.hpp"
#include "config/ServerConfig.hpp"
#include <string>
#include <vector>

LocationConfig LocationBuilder::build(const ParsedLocation& parsed,
                                      const ServerConfig& config)
{
  LocationConfig location(config);

  const ParsedConfig::Directive& directives = parsed.getDirective();
  for (ParsedConfig::Directive::const_iterator it = directives.begin();
       it != directives.end();
       ++it) {
    const std::string& key = it->first;
    const std::vector<std::string>& values = it->second;
    checkDirectiveHandler(key, value, location);
  }

  return location;
}
