#ifndef PARSEDSERVER_HPP
#define PARSEDSERVER_HPP

#include "ParsedLocation.hpp"
#include <map>
#include <string>
#include <vector>

class ParsedServer
{
public:
  std::map<std::string, std::vector<std::string> >& getDirective();
  const std::map<std::string, std::vector<std::string> >& getDirective() const;

  std::vector<ParsedLocation>& getLocations();
  const std::vector<ParsedLocation>& getLocations() const;

private:
  std::map<std::string, std::vector<std::string> > _directives;
  std::vector<ParsedLocation> _locations;
};

#endif
