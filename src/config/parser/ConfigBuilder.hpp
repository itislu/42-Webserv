#ifndef CONFIGBUILDER_HPP
#define CONFIGBUILDER_HPP

#include "ParsedConfig.hpp"
#include "ParsedServer.hpp"
#include <config/Config.hpp>
#include <vector>

namespace config {

class ConfigBuilder
{
public:
  static void build(const ParsedConfig& parsed);

private:
  static void buildServers(const std::vector<ParsedServer>& servers,
                           Config& config);
};

} // namespace config

#endif
