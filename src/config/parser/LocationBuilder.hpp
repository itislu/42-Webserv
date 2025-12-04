#ifndef LOCATIONBUILDER_HPP
#define LOCATIONBUILDER_HPP

#include "ParsedLocation.hpp"
#include <config/LocationConfig.hpp>

class ServerConfig;

namespace config {

class LocationBuilder
{
public:
  static LocationConfig build(const ParsedLocation& parsed,
                              const ServerConfig& config);
};

} // namespace config

#endif
