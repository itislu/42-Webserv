#ifndef PARSEDCONFIG_HPP
#define PARSEDCONFIG_HPP

#include "ParsedServer.hpp"
#include <map>
#include <ostream>
#include <string>
#include <vector>

class ParsedConfig
{
public:
  typedef std::map<std::string, std::vector<std::string> > Directive;

  Directive& getDirective();
  const Directive& getDirective() const;

  std::vector<ParsedServer>& getServers();
  const std::vector<ParsedServer>& getServers() const;

  void addServer(const ParsedServer& server);
  void addDirective(const Directive& directive);

private:
  std::vector<ParsedServer> _servers;
  Directive _directives;
};

std::ostream& operator<<(std::ostream& out, ParsedConfig& parsed);

#endif
