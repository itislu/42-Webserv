#include "ServerBuilder.hpp"
#include "config/LocationBuilder.hpp"
#include "config/ParsedConfig.hpp"
#include "config/ParsedLocation.hpp"
#include "config/ParsedServer.hpp"
#include "config/ServerConfig.hpp"
#include <vector>

ServerConfig ServerBuilder::build(const ParsedServer& parsed,
                                  const Config& config)
{
  ServerConfig server(config);

  const ParsedConfig::Directive& directives = parsed.getDirective();
  for (ParsedConfig::Directive::const_iterator it = directives.begin();
       it != directives.end();
       ++it) {
    // check duplicates only allowed for listen with different port
    // check and set values
  }

  // BUILD LOCATIONS
  const std::vector<ParsedLocation>& locations = parsed.getLocations();
  for (std::vector<ParsedLocation>::const_iterator it = locations.begin();
       it != locations.end();
       ++it) {
    server.addLocation(LocationBuilder::build(*it, server));
  }
  return server;
}
