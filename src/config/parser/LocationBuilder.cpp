#include "LocationBuilder.hpp"
#include "DirectiveHandler.hpp"
#include "ParsedLocation.hpp"
#include <config/LocationConfig.hpp>
#include <config/ServerConfig.hpp>

namespace config {

LocationConfig LocationBuilder::build(const ParsedLocation& parsed,
                                      const ServerConfig& config)
{
  LocationConfig location(config);
  location.setPath(parsed.getPath());
  DirectiveHandler<LocationConfig>::buildDirectives(parsed.getDirectives(),
                                                    location);

  return location;
}

} // namespace config
