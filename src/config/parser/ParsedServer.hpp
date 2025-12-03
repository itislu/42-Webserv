#ifndef PARSEDSERVER_HPP
#define PARSEDSERVER_HPP

#include "ConfigTypes.hpp"
#include "ParsedLocation.hpp"
#include <vector>

namespace config {

class ParsedServer
{
public:
  DirectiveMap& getDirectives();
  const DirectiveMap& getDirectives() const;

  std::vector<ParsedLocation>& getLocations();
  const std::vector<ParsedLocation>& getLocations() const;

private:
  DirectiveMap _directives;
  std::vector<ParsedLocation> _locations;
};

} // namespace config

#endif
