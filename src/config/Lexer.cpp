#include "Lexer.hpp"
#include "config/FileUtils.hpp"
#include "config/Token.hpp"
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>

// Skip spaces and newlines.
// Skip comments (# ...).
// If it finds {, }, or ;, it returns those as individual tokens.
// Otherwise, it reads a word (like listen, 8080, or /var/www/html) and returns
// it as an IDENT token.

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

  _input.assign(std::istreambuf_iterator<char>(_file),
                std::istreambuf_iterator<char>());
}

void Lexer::validateInputFile()
{
  if (!fileutils::checkFileExtension(_filepath, ".conf")) {
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
    token.setType(END);
    return token;
  }

  const unsigned char chr = static_cast<unsigned char>(_input[_pos]);

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

  if (chr == '#') {
    ++_pos;
    token.setType(COMMENT);
    return token;
  }

  if (isspace(chr) == 0) {
    std::size_t idx = _pos;
    while (idx < _input.size()) {
      const unsigned char curr = static_cast<unsigned char>(_input[idx]);
      if ((isspace(curr) != 0) || curr == '{' || curr == '}' || curr == ';') {
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
  while (_pos < _input.size() &&
         (isspace(static_cast<unsigned char>(_input[_pos])) != 0)) {
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
