#ifndef LOCATIONBUILDER_HPP
#define LOCATIONBUILDER_HPP

#include "config/LocationConfig.hpp"
#include "config/ParsedLocation.hpp"
#include "config/ServerConfig.hpp"

namespace config {

class LocationBuilder
{
public:
  static LocationConfig build(const ParsedLocation& parsed,
                              const ServerConfig& config);
};

} // namespace config

#endif
