#include "ConfigBuilder.hpp"
#include "config/Config.hpp"
#include "config/DirectiveHandler.hpp"
#include "config/ParsedConfig.hpp"
#include "config/ParsedServer.hpp"
#include "config/ServerBuilder.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

/*
  - no duplicates in global directive allowed
  - every directive must have only one value
  - must have directives:
    - timeout
    - maxbodysize
    -
  - at least 1 server

*/

void ConfigBuilder::buildServers(const std::vector<ParsedServer>& servers,
                                 Config& config)
{
  /* Nginx uses default server - sure if we want that too */
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

  DirectiveHandler<Config>::buildDirectives(parsed.getDirective(), config);
  // buildServers(parsed.getServers(), config);
  config.setDefaultTimeout();

  std::cout << config;

  return config;
}
