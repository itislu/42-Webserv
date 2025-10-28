#include "ConfigParser.hpp"
#include "FileUtils.hpp"
#include "config/Lexer.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

// Directives end with ;
// Blocks are wrapped in { } (like server { ... }, location { ... })
// Comments start with # - ignore rest of the line

ConfigParser::ConfigParser(const char* path)
{
  _filepath = static_cast<std::string>(path);
}

void ConfigParser::validateInputFile() const
{
  if (!checkFileExtension(_filepath, ".conf")) {
    throw std::invalid_argument("invalid file extension: " + _filepath);
  }

  if (!isFile(_filepath)) {
    throw std::invalid_argument("invalid file not a file: " + _filepath);
  }

  std::ifstream file(_filepath.c_str());
  if (!file.is_open()) {
    throw std::invalid_argument("couldn't open configfile: " + _filepath);
  }
}

void ConfigParser::parse()
{
  Lexer lexer(_filepath);
  Token token = lexer.next();

  while (token.type != END) {
    
    token = lexer.next();
  }
}

void ConfigParser::readConfig()
{
  validateInputFile();
  parse();
}
