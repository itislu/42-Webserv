#include "Lexer.hpp"
#include "config/FileUtils.hpp"
#include "config/Token.hpp"
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <libftpp/string.hpp>
#include <stdexcept>
#include <string>

namespace config {

/*
- Skips whitespace character (spaces, newlines, ...).
- Skips comments that start with '#' and continue to the end of the line.
- If it finds {, }, or ;, it returns those as individual tokens.
- Otherwise, it reads a word (like listen, 8080, or /var/www/html) and returns
it as an Token::Ident token.
*/

Lexer::Lexer(const std::string& file)
  : _filepath(file)
  , _pos(0)
  , _line(1)
{
  init();
}

void Lexer::init()
{
  validateInputFile();

  _input.assign((std::istreambuf_iterator<char>(_file)),
                std::istreambuf_iterator<char>());
}

void Lexer::validateInputFile()
{
  if (!ft::ends_with(_filepath, ".conf")) {
    throw std::invalid_argument("invalid file extension: " + _filepath);
  }

  if (!fileutils::isFile(_filepath)) {
    throw std::invalid_argument("invalid file: " + _filepath);
  }

  _file.open(_filepath.c_str());
  if (!_file.is_open()) {
    throw std::runtime_error("Failed to open input file");
  }
}

std::size_t Lexer::getLineNum() const
{
  return _line;
}

Token Lexer::next()
{
  skipWhiteSpaces();

  Token token(_line);
  if (_pos >= _input.size()) {
    token.setType(Token::End);
    return token;
  }

  const char chr = _input[_pos];

  if (chr == '{') {
    ++_pos;
    token.setType(Token::LBrace);
    return token;
  }

  if (chr == '}') {
    ++_pos;
    token.setType(Token::RBrace);
    return token;
  }

  if (chr == ';') {
    ++_pos;
    token.setType(Token::Semicolon);
    return token;
  }

  if (chr == '#') {
    ++_pos;
    token.setType(Token::Comment);
    return token;
  }

  if (std::isspace(chr) == 0) {
    std::size_t idx = _pos;
    while (idx < _input.size()) {
      const char curr = _input[idx];
      if ((std::isspace(curr) != 0) || curr == '{' || curr == '}' ||
          curr == ';') {
        break;
      }
      ++idx;
    }
    const std::string value = _input.substr(_pos, idx - _pos);
    token.setValue(value);
    token.setType(Token::Ident);
    _pos = idx;
    return token;
  }
  return token;
}

void Lexer::skipWhiteSpaces()
{
  while (_pos < _input.size() && (std::isspace(_input[_pos]) != 0)) {
    if (_input[_pos] == '\n') {
      ++_line;
    }
    ++_pos;
  }
}

void Lexer::skipComment()
{
  while (_pos < _input.size()) {
    if (_input[_pos] == '\n') {
      ++_pos;
      ++_line;
      break;
    }
    ++_pos;
  }
}

} // namespace config
