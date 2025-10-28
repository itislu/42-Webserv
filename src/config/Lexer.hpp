#ifndef LEXER_HPP
#define LEXER_HPP

#include <cstddef>
#include <fstream>
#include <string>

enum e_type
{
  IDENT,
  SEMICOLON,
  LBRACE,
  RBRACE,
  END
};

struct Token
{
  int type;
  std::string value;
  int line;
};

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