#ifndef PARSEDSERVER_HPP
#define PARSEDSERVER_HPP

#include "ParsedLocation.hpp"
#include "config/ConfigTypes.hpp"
#include <map>
#include <string>
#include <vector>

class ParsedServer
{
public:
  typedef std::map<std::string, std::vector<std::string> > Directive;

  DirectiveMap& getDirective();
  const DirectiveMap& getDirective() const;

  std::vector<ParsedLocation>& getLocations();
  const std::vector<ParsedLocation>& getLocations() const;

private:
  DirectiveMap _directives;
  std::vector<ParsedLocation> _locations;
};

#endif
