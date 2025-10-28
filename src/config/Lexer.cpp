#include "Lexer.hpp"
#include <sstream>
#include <stdexcept>
#include <string>

// Skip spaces and newlines.
// Skip comments (# ...).
// If it finds {, }, or ;, it returns those as individual tokens.
// Otherwise, it reads a word (like listen, 8080, or /var/www/html) and returns
// it as an IDENT token.

Lexer::Lexer(const std::string& file)
  : _pos(0)
  , _line(1)
{
  _file.open(file.c_str());
  if (!_file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }
  init();
}

void Lexer::init()
{
  std::stringstream buffer;
  buffer << _file.rdbuf();
  _input = buffer.str();
}

Token Lexer::next()
{
  Token const token;
  return token;
}

void Lexer::skipWhiteSpaces() {}

void Lexer::skipComments() {}
