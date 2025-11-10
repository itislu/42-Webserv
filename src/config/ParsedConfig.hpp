#ifndef PARSEDCONFIG_HPP
#define PARSEDCONFIG_HPP

#include "ParsedServer.hpp"
#include "config/ConfigTypes.hpp"
#include <ostream>
#include <vector>

class ParsedConfig
{
public:

  DirectiveMap& getDirective();
  const DirectiveMap& getDirective() const;

  std::vector<ParsedServer>& getServers();
  const std::vector<ParsedServer>& getServers() const;

  void addServer(const ParsedServer& server);
  void addDirective(const DirectiveMap& directive);

private:
  std::vector<ParsedServer> _servers;
  DirectiveMap _directives;
};

std::ostream& operator<<(std::ostream& out, ParsedConfig& parsed);

#endif
