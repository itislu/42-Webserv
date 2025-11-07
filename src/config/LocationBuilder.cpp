#include "LocationBuilder.hpp"
#include "config/DirectiveHandler.hpp"
#include "config/LocationConfig.hpp"
#include "config/ParsedLocation.hpp"
#include "config/ServerConfig.hpp"

LocationConfig LocationBuilder::build(const ParsedLocation& parsed,
                                      const ServerConfig& config)
{
  LocationConfig location(config);
  DirectiveHandler<LocationConfig>::buildDirectives(parsed.getDirective(),
                                                    location);

  return location;
}
