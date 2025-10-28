#include "Lexer.hpp"
#include "config/Token.hpp"
#include <cctype>
#include <cstddef>
#include <iostream>
#include <ostream>
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
  skipWhiteSpaces();

  Token token(_line);
  if (_pos >= _input.size()) {
    token.setType(END);
    return token;
  }

  const char chr = _input[_pos];
  std::cout << "Char: " << chr << "\n";

  if (chr == '{') {
    ++_pos;
    token.setType(LBRACE);
    return token;
  }

  if (chr == '}') {
    ++_pos;
    token.setType(RBRACE);
    return token;
  }

  if (chr == ';') {
    ++_pos;
    token.setType(SEMICOLON);
    return token;
  }

  if (isspace(static_cast<unsigned char>(chr)) == 0) {
    std::size_t idx = _pos;
    while (idx < _input.size()) {
      const unsigned char curr = _input[idx];
      if ((isspace(curr) != 0) || curr == '{' || curr == '}' || curr == ';' ||
          curr == '#') {
        break;
      }
      ++idx;
    }
    const std::string value = _input.substr(_pos, idx - _pos);
    token.setValue(value);
    token.setType(IDENT);
    _pos = idx;
    return token;
  }
  return token;
}

void Lexer::skipWhiteSpaces()
{
  while (_pos < _input.size() && (isspace(_input[_pos]) != 0)) {
    if (_input[_pos] == '\n') {
      ++_line;
    }
    ++_pos;
  }
}

void Lexer::skipComments()
{
  // TODO: implement
}
