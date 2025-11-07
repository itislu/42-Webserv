#include "ConfigParser.hpp"
#include "config/Config.hpp"
#include "config/ConfigBuilder.hpp"
#include "config/Lexer.hpp"
#include "config/ParsedConfig.hpp"
#include "config/ParsedLocation.hpp"
#include "config/ParsedServer.hpp"
#include "config/Token.hpp"
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Directives end with ;
// Blocks are wrapped in { } (like server { ... }, location { ... })
// Comments start with # - ignore rest of the line

ConfigParser::ConfigParser(const char* path)
  : _filepath(path)
  , _lexer(_filepath)
  , _token(1)
{
}

void ConfigParser::validateParsedConfig() const
{
  // TODO: implement
}

bool ConfigParser::isExpectedNext(e_type type)
{
  return _token.getType() == type;
}

void ConfigParser::skipComments()
{
  _token = _lexer.next();
  while (_token.getType() == COMMENT && _token.getType() != END) {
    _lexer.skipComment();
    _token = _lexer.next();
  }
}

void ConfigParser::invalidToken(const std::string& err) const
{
  std::ostringstream oss;
  oss << "invalid token in " << err << " at line " << _lexer.getLine() << ": '"
      << _token.getValue() << "'";
  throw std::runtime_error(oss.str());
}

void ConfigParser::parseDirective(
  std::map<std::string, std::vector<std::string> >& directive)
{
  const std::string key = _token.getValue();
  std::vector<std::string> value;
  _token = _lexer.next();
  while (_token.getType() != SEMICOLON && _token.getType() != END) {
    value.push_back(_token.getValue());
    _token = _lexer.next();
  }
  if (_token.getType() != SEMICOLON) {
    invalidToken("parse directive - missing ';'");
  }

  /* TODO: check for duplicates before inserting, duplicates only allowed for
   * certain keys */

  directive[key] = value;
}

void ConfigParser::parseLocationConfig(ParsedServer& server)
{
  skipComments();

  if (!isExpectedNext(IDENT)) {
    invalidToken("location config");
  }

  ParsedLocation location(_token.getValue());

  skipComments();

  if (!isExpectedNext(LBRACE)) {
    invalidToken("location config");
  }

  for (_token = _lexer.next();
       _token.getType() != RBRACE && _token.getType() != END;
       _token = _lexer.next()) {
    if (_token.getType() == COMMENT) {
      _lexer.skipComment();
    } else if (_token.getType() == IDENT) {
      parseDirective(location.getDirective());
    } else {
      invalidToken("location config");
    }
  }

  if (_token.getType() != RBRACE) {
    invalidToken("location config - missing '}'");
  }

  server.getLocations().push_back(location);
}

void ConfigParser::parseServerConfig()
{
  ParsedServer server;

  skipComments();

  if (!isExpectedNext(LBRACE)) {
    invalidToken("server config - missing '{'");
  }

  for (_token = _lexer.next();
       _token.getType() != RBRACE && _token.getType() != END;
       _token = _lexer.next()) {
    if (_token.getType() == COMMENT) {
      _lexer.skipComment();
    } else if (_token.getType() == IDENT && _token.getValue() == "location") {
      parseLocationConfig(server);
    } else if (_token.getType() == IDENT) {
      parseDirective(server.getDirective());
    } else {
      invalidToken("server config");
    }
  }
  if (_token.getType() != RBRACE) {
    invalidToken("server config - missing '}'");
  }

  _parsed.addServer(server);
}

void ConfigParser::parse()
{
  for (_token = _lexer.next(); _token.getType() != END;
       _token = _lexer.next()) {
    if (_token.getType() == COMMENT) {
      _lexer.skipComment();
    } else if (_token.getType() == IDENT && _token.getValue() == "server") {
      parseServerConfig();
    } else if (_token.getType() == IDENT) {
      parseDirective(_parsed.getDirective());
    } else {
      invalidToken("global config");
    }
  }
}

void ConfigParser::readConfig()
{
  _lexer.init();
  parse();
  std::cout << _parsed;

  std::cout << "BUILD!\n";
  const Config conf = ConfigBuilder::build(_parsed);
  std::cout << conf;
  std::cout << "END BUILD!\n";
}
