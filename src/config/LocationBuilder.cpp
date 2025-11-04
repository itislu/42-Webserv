#include "LocationBuilder.hpp"
#include "config/LocationConfig.hpp"
#include "config/ParsedConfig.hpp"
#include "config/ParsedLocation.hpp"
#include "config/ServerConfig.hpp"
#include <iostream>
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
    if (key == "root" && !values.empty()) {
      location.setRoot(values[0]);
    } else if (key == "index" && !values.empty()) {
      location.setIndex(values[0]);
    } else if (key == "allow_methods") {
      for (std::vector<std::string>::const_iterator vit = values.begin();
           vit != values.end();
           ++vit) {
        location.addAllowedMethod(*vit);
      }
    } else {
      std::cerr << "Warning: unknown directive in location: " << key << "\n";
    }
  }

  return location;
}
