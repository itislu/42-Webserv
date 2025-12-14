#ifndef PARSEDCONFIG_HPP
#define PARSEDCONFIG_HPP

#include "ConfigTypes.hpp"
#include "ParsedServer.hpp"
#include <ostream>
#include <vector>

namespace config {

class ParsedConfig
{
public:
  DirectiveMap& getDirectives();
  const DirectiveMap& getDirectives() const;

  std::vector<ParsedServer>& getServers();
  const std::vector<ParsedServer>& getServers() const;

  void addServer(const ParsedServer& server);
  void addDirective(const DirectiveMap& directive);

private:
  std::vector<ParsedServer> _servers;
  DirectiveMap _directives;
};

std::ostream& operator<<(std::ostream& out, const ParsedConfig& parsed);

} // namespace config

#endif
