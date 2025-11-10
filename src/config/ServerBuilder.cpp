#include "ServerBuilder.hpp"
#include "config/DirectiveHandler.hpp"
#include "config/LocationBuilder.hpp"
#include "config/ParsedLocation.hpp"
#include "config/ParsedServer.hpp"
#include "config/ServerConfig.hpp"
#include <cstring>
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
  /* TODO: maybe add a check if any server/port combination is duplicated */
  ServerConfig server(config);

  DirectiveHandler<ServerConfig>::buildDirectives(parsed.getDirective(),
                                                  server);

  buildLocations(parsed.getLocations(), server);

  return server;
}
