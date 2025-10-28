#ifndef LEXER_HPP
#define LEXER_HPP

#include "Token.hpp"
#include <cstddef>
#include <fstream>
#include <string>

class Lexer
{
public:
  explicit Lexer(const std::string& file);

  Token next();

private:
  void init();
  void skipWhiteSpaces();
  void skipComments();

  std::ifstream _file;
  std::string _input;
  std::size_t _pos;
  std::size_t _line;
};

#endif
