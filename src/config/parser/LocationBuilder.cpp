#include "LocationBuilder.hpp"
#include "DirectiveHandler.hpp"
#include "ParsedLocation.hpp"
#include "config/parser/ConfigTypes.hpp"
#include <config/LocationConfig.hpp>
#include <config/ServerConfig.hpp>
#include <stdexcept>
#include <string>

namespace config {

void LocationBuilder::_validateDependentDirectives(
  const LocationConfig& location,
  const DirectiveMap& directives)
{
  DirectiveMap::const_iterator iter;

  iter = directives.find("redirect");
  if (iter != directives.end()) {
    iter = directives.find("redirect_url");
    if (iter == directives.end()) {
      throw std::invalid_argument("location \"" +
                                  std::string(location.getPath()) +
                                  "\" is missing redirect_url");
    }
    iter = directives.find("redirect_code");
    if (iter == directives.end()) {
      throw std::invalid_argument("location \"" +
                                  std::string(location.getPath()) +
                                  "\" is missing redirect_code");
    }
  }
}

LocationConfig LocationBuilder::build(const ParsedLocation& parsed,
                                      const ServerConfig& config)
{
  LocationConfig location(config);
  location.setPath(parsed.getPath());
  _validateDependentDirectives(location, parsed.getDirectives());
  DirectiveHandler<LocationConfig>::buildDirectives(parsed.getDirectives(),
                                                    location);

  return location;
}

} // namespace config
