#include "ConfigParser.hpp"
#include "config/Config.hpp"
#include "config/ConfigBuilder.hpp"
#include "config/ConfigTypes.hpp"
#include "config/Converters.hpp"
#include "config/Lexer.hpp"
#include "config/ParsedConfig.hpp"
#include "config/ParsedLocation.hpp"
#include "config/ParsedServer.hpp"
#include "config/Token.hpp"
#include <cstddef>
#include <exception>
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
  oss << "invalid token in " << err << " at line " << _lexer.getLineNum()
      << ": '" << _token.getValue() << "'";
  throw std::invalid_argument(oss.str());
}

bool ConfigParser::isRepeatableDirective(const std::string& key)
{
  return (key == "listen" || key == "server_name" || key == "error_page");
}

void ConfigParser::validateErrorPages(const std::vector<std::string>& value)
{
  if (value.size() < 2) {
    throw std::invalid_argument("invalid number of arguments");
  }

  // Validate all but last are codes
  for (std::size_t i = 0; i < value.size() - 1; ++i) {
    convert::toCode(value[i]);
  }

  // Validate last one is NOT a code
  bool isCode = true;
  try {
    convert::toCode(value.back());
  } catch (const std::exception& e) {
    isCode = false;
  }
  if (isCode) {
    throw std::invalid_argument("last argument must be a path, not a code");
  }
}

void ConfigParser::addToDirective(DirectiveMap& directive,
                                  const std::string& key,
                                  const std::vector<std::string>& value)
{
  if (key == "error_page") {
    try {
      validateErrorPages(value);
    } catch (const std::exception& e) {
      throw std::invalid_argument("error_page: " + std::string(e.what()));
    }
  }

  const DirectiveMap::iterator iter = directive.find(key);
  if (iter != directive.end()) {
    if (isRepeatableDirective(key)) {
      iter->second.insert(iter->second.end(), value.begin(), value.end());
    } else {
      throw std::invalid_argument("duplicate directive: " + key);
    }
  } else {
    directive[key] = value;
  }
}

void ConfigParser::parseDirective(DirectiveMap& directive)
{
  const std::string key = _token.getValue();
  std::vector<std::string> value;
  _token = _lexer.next();
  while (_token.getType() != SEMICOLON && _token.getType() != END) {
    if (_token.getType() == COMMENT) {
      _lexer.skipComment();
    } else {
      value.push_back(_token.getValue());
    }
    _token = _lexer.next();
  }
  if (_token.getType() != SEMICOLON) {
    invalidToken("parse directive - missing ';'");
  }
  addToDirective(directive, key, value);
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
      parseDirective(location.getDirectives());
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
      parseDirective(server.getDirectives());
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
      parseDirective(_parsed.getDirectives());
    } else {
      invalidToken("global config");
    }
  }
}

Config ConfigParser::parseConfig()
{
  // parse
  parse();

  // build
  return ConfigBuilder::build(_parsed);
}

// error_page 505 505.html
// error_page 404 403 402 401 40x.html
