#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "config/Lexer.hpp"
#include "config/ParsedConfig.hpp"
#include "config/ParsedServer.hpp"
#include "config/Token.hpp"
#include <map>
#include <string>
#include <vector>
class ConfigParser
{
public:
  explicit ConfigParser(const char* path);
  void readConfig();

private:
  void parse();
  void parseServerConfig();
  void parseLocationConfig(ParsedServer& server);
  void parseDirective(
    std::map<std::string, std::vector<std::string> >& directives);
  bool isExpectedNext(e_type type);
  void invalidToken(const std::string& err) const;
  void validateParsedConfig() const;

  std::string _filepath;
  Lexer _lexer;
  Token _token;
  ParsedConfig _parsed;
};

/* TODO: */
void printParsedConfig(ParsedConfig& parsed);

#endif
