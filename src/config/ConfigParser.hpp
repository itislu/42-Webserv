#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "config/Config.hpp"
#include "config/ConfigTypes.hpp"
#include "config/Lexer.hpp"
#include "config/ParsedConfig.hpp"
#include "config/ParsedServer.hpp"
#include "config/Token.hpp"
#include <string>
#include <vector>

namespace config {

class ConfigParser
{
public:
  explicit ConfigParser(const char* path);
  Config parseConfig();

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

  bool isExpectedNext(Token::Type type);
  void invalidToken(const std::string& err) const;
  void skipComments();

  std::string _filepath;
  Lexer _lexer;
  Token _token;
  ParsedConfig _parsed;
};

} // namespace config

#endif
