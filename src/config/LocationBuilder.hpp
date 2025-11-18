#ifndef LOCATIONBUILDER_HPP
#define LOCATIONBUILDER_HPP

#include "config/LocationConfig.hpp"
#include "config/ParsedLocation.hpp"

namespace config {

class ServerConfig;

class LocationBuilder
{
public:
  static LocationConfig build(const ParsedLocation& parsed,
                              const ServerConfig& config);
};

} // namespace config

#endif
