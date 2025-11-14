#ifndef LOCATIONBUILDER_HPP
#define LOCATIONBUILDER_HPP

#include "config/LocationConfig.hpp"
#include "config/ParsedLocation.hpp"

class ServerConfig;

class LocationBuilder
{
public:
  static LocationConfig build(const ParsedLocation& parsed,
                              const ServerConfig& config);

private:
};

#endif
