#ifndef SERVERBUILDER_HPP
#define SERVERBUILDER_HPP

#include "config/ConfigTypes.hpp"
#include "config/ParsedLocation.hpp"
#include "config/ParsedServer.hpp"
#include "config/ServerConfig.hpp"
#include <vector>

namespace config {

class ServerBuilder
{
public:
  static ServerConfig build(const ParsedServer& parsed, const Config& config);

private:
  static void buildLocations(const std::vector<ParsedLocation>& locations,
                             ServerConfig& server);
  static void validateMandatoryDirectives(const DirectiveMap& directives);
  static void validateLocations(const std::vector<ParsedLocation>& locations);
};

} // namespace config

#endif
