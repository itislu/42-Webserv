#include "ConfigBuilder.hpp"
#include "config/Config.hpp"
#include "config/DirectiveHandler.hpp"
#include "config/ParsedConfig.hpp"
#include "config/ParsedServer.hpp"
#include "config/ServerBuilder.hpp"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

namespace config {

/*
  - no duplicates in global directive allowed
  - every directive must have only one value
  - at least 1 server (nginx uses a default server if nothing is in the config)
*/

void ConfigBuilder::buildServers(const std::vector<ParsedServer>& servers,
                                 Config& config)
{
  if (servers.empty()) {
    throw std::invalid_argument("No server in config");
  }
  for (std::vector<ParsedServer>::const_iterator it = servers.begin();
       it != servers.end();
       ++it) {
    config.addServer(ServerBuilder::build(*it, config));
  }
}

Config ConfigBuilder::build(const ParsedConfig& parsed)
{
  Config config;

  DirectiveHandler<Config>::buildDirectives(parsed.getDirectives(), config);
  buildServers(parsed.getServers(), config);
  config.setDefaultTimeout();

  return config;
}

} // namespace config
