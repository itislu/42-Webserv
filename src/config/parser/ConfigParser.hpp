#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "ConfigTypes.hpp"
#include "Lexer.hpp"
#include "ParsedConfig.hpp"
#include "ParsedServer.hpp"
#include "Token.hpp"
#include <string>
#include <vector>

namespace config {

class ConfigParser
{
public:
  explicit ConfigParser(const char* path);
  void parseConfig();

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
