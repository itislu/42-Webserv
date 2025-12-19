#ifndef LOCATIONBUILDER_HPP
#define LOCATIONBUILDER_HPP

#include "ParsedLocation.hpp"
#include "config/parser/ConfigTypes.hpp"
#include <config/LocationConfig.hpp>
#include <config/ServerConfig.hpp>

namespace config {

class LocationBuilder
{
public:
  static LocationConfig build(const ParsedLocation& parsed,
                              const ServerConfig& config);

private:
  static void _validateDependentDirectives(const LocationConfig& location,
                                           const DirectiveMap& directives);
};

} // namespace config

#endif
