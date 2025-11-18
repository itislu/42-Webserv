#ifndef LEXER_HPP
#define LEXER_HPP

#include "Token.hpp"
#include <cstddef>
#include <fstream>
#include <string>

namespace config {

class Lexer
{
public:
  explicit Lexer(const std::string& file);

  Token next();
  std::size_t getLineNum() const;
  void skipComment();

private:
  void init();
  void validateInputFile();
  void skipWhiteSpaces();

  std::ifstream _file;
  std::string _filepath;
  std::string _input;
  std::size_t _pos;
  std::size_t _line;
};

} // namespace config

#endif
