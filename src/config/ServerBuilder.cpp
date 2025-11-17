#include "ServerBuilder.hpp"
#include "config/ConfigTypes.hpp"
#include "config/DirectiveHandler.hpp"
#include "config/LocationBuilder.hpp"
#include "config/ParsedLocation.hpp"
#include "config/ParsedServer.hpp"
#include "config/ServerConfig.hpp"
#include <cstring>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace config {

void ServerBuilder::validateMandatoryDirectives(const DirectiveMap& directives)
{
  DirectiveMap::const_iterator iter;

  iter = directives.find("listen");
  if (iter == directives.end()) {
    throw std::invalid_argument("server - is missing 'listen' directive");
  }

  iter = directives.find("server_name");
  if (iter == directives.end()) {
    throw std::invalid_argument("server - is missing 'server_name' directive");
  }
}

/* checks if any location is duplicated in that server */
void ServerBuilder::validateLocations(
  const std::vector<ParsedLocation>& locations)
{
  std::set<std::string> checked;
  for (std::vector<ParsedLocation>::const_iterator it = locations.begin();
       it != locations.end();
       ++it) {
    const std::string& path = it->getPath();
    if (!checked.insert(path).second) {
      throw std::invalid_argument("server - duplicated location directive " +
                                  path);
    }
  }
}

void ServerBuilder::buildLocations(const std::vector<ParsedLocation>& locations,
                                   ServerConfig& server)
{
  validateLocations(locations);

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

  validateMandatoryDirectives(parsed.getDirectives());

  ServerConfig server(config);
  DirectiveHandler<ServerConfig>::buildDirectives(parsed.getDirectives(),
                                                  server);
  buildLocations(parsed.getLocations(), server);

  return server;
}

} // namespace config
