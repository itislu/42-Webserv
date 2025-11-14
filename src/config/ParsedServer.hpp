#ifndef PARSEDSERVER_HPP
#define PARSEDSERVER_HPP

#include "ParsedLocation.hpp"
#include "config/ConfigTypes.hpp"
#include <vector>

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

#endif
