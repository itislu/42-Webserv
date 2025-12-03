#include "ConfigBuilder.hpp"
#include "DirectiveHandler.hpp"
#include "ParsedConfig.hpp"
#include "ParsedServer.hpp"
#include "ServerBuilder.hpp"
#include <config/Config.hpp>
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

void ConfigBuilder::build(const ParsedConfig& parsed)
{
  Config::getConfig();

  DirectiveHandler<Config>::buildDirectives(parsed.getDirectives(),
                                            Config::getConfig());
  buildServers(parsed.getServers(), Config::getConfig());

  Config::getConfig().setDefaultTimeout();
}

} // namespace config
