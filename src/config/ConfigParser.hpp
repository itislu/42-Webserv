#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "config/ConfigTypes.hpp"
#include "config/Lexer.hpp"
#include "config/ParsedConfig.hpp"
#include "config/ParsedServer.hpp"
#include "config/Token.hpp"
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
  void parseDirective(DirectiveMap& directives);


  static void validateErrorPages(const std::vector<std::string>& value);
  static void addToDirective(DirectiveMap& directive,
                      const std::string& key,
                      const std::vector<std::string>& value);
  static bool isRepeatableDirective(const std::string& key);

  bool isExpectedNext(e_type type);
  void invalidToken(const std::string& err) const;
  void validateParsedConfig() const;
  void skipComments();

  std::string _filepath;
  Lexer _lexer;
  Token _token;
  ParsedConfig _parsed;
};

#endif
