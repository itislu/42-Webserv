#ifndef SERVERBUILDER_HPP
#define SERVERBUILDER_HPP

#include "config/ParsedLocation.hpp"
#include "config/ParsedServer.hpp"
#include "config/ServerConfig.hpp"
#include <vector>

class ServerBuilder
{
public:
  static ServerConfig build(const ParsedServer& parsed, const Config& config);

private:
  static void buildLocations(const std::vector<ParsedLocation>& locations,
                             ServerConfig& server);
};

#endif
