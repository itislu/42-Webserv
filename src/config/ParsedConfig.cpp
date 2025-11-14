#include "ParsedConfig.hpp"
#include "config/ConfigTypes.hpp"
#include "config/ParsedLocation.hpp"
#include "config/ParsedServer.hpp"
#include <map>
#include <ostream>
#include <string>
#include <vector>

DirectiveMap& ParsedConfig::getDirectives()
{
  return _directives;
}

const DirectiveMap& ParsedConfig::getDirectives() const
{
  return _directives;
}

std::vector<ParsedServer>& ParsedConfig::getServers()
{
  return _servers;
}

const std::vector<ParsedServer>& ParsedConfig::getServers() const
{
  return _servers;
}

void ParsedConfig::addServer(const ParsedServer& server)
{
  _servers.push_back(server);
}

void ParsedConfig::addDirective(const DirectiveMap& directive)
{
  DirectiveMap::const_iterator iter = directive.begin();
  for (; iter != directive.end(); ++iter) {
    const std::string& key = iter->first;
    const std::vector<std::string>& values = iter->second;

    // if key already exists -> append
    if (_directives.find(key) != _directives.end()) {
      std::vector<std::string>& existing = _directives[key];
      existing.insert(existing.end(), values.begin(), values.end());
    } else {
      // otherwise, just insert new entry
      _directives[key] = values;
    }
  }
}

/* Print functions - delete later */
static void printIndent(int level, std::ostream& out)
{
  for (int i = 0; i < level; ++i) {
    out << "  ";
  }
}

static void printDirectiveMap(const DirectiveMap& directives,
                              int indent,
                              std::ostream& out)
{
  DirectiveMap::const_iterator iter;
  for (iter = directives.begin(); iter != directives.end(); ++iter) {
    printIndent(indent, out);
    out << iter->first;
    const std::vector<std::string>& values = iter->second;
    for (std::vector<std::string>::const_iterator val = values.begin();
         val != values.end();
         ++val) {
      out << " " << *val;
    }
    out << ";" << "\n";
  }
}

std::ostream& operator<<(std::ostream& out, const ParsedConfig& parsed)
{
  out << "==============================" << "\n";
  out << " Global Configuration" << "\n";
  out << "==============================" << "\n";
  printDirectiveMap(parsed.getDirectives(), 1, out);

  const std::vector<ParsedServer>& servers = parsed.getServers();
  for (std::vector<ParsedServer>::const_iterator servIt = servers.begin();
       servIt != servers.end();
       ++servIt) {
    out << "\n";
    printIndent(0, out);
    out << "server {" << "\n";

    printDirectiveMap(servIt->getDirectives(), 1, out);

    const std::vector<ParsedLocation>& locs = servIt->getLocations();
    for (std::vector<ParsedLocation>::const_iterator locIt = locs.begin();
         locIt != locs.end();
         ++locIt) {
      printIndent(1, out);
      out << "location " << locIt->getPath() << " {" << "\n";
      printDirectiveMap(locIt->getDirectives(), 2, out);
      printIndent(1, out);
      out << "}" << "\n";
    }

    printIndent(0, out);
    out << "}" << "\n";
  }

  out << "\n";
  out << "End of configuration" << "\n";
  return out;
}
