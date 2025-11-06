#include "ServerBuilder.hpp"
#include "config/ConfigBuilder.hpp"
#include "config/LocationBuilder.hpp"
#include "config/ParsedConfig.hpp"
#include "config/ParsedLocation.hpp"
#include "config/ParsedServer.hpp"
#include "config/ServerConfig.hpp"
#include <cstring>
#include <string>
#include <vector>

void ServerBuilder::buildLocations(const std::vector<ParsedLocation>& locations,
                                   ServerConfig& server)
{
  for (std::vector<ParsedLocation>::const_iterator it = locations.begin();
       it != locations.end();
       ++it) {
    server.addLocation(LocationBuilder::build(*it, server));
  }
}

ServerConfig ServerBuilder::build(const ParsedServer& parsed,
                                  const Config& config)
{
  ServerConfig server(config);

  const ParsedConfig::Directive& directives = parsed.getDirective();
  for (ParsedConfig::Directive::const_iterator it = directives.begin();
       it != directives.end();
       ++it) {
  }
  buildLocations(parsed.getLocations(), server);

  return server;
}
