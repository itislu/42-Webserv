#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <cstddef>
#include <ostream>
#include <string>

enum e_type
{
  INVALID,
  IDENT,
  SEMICOLON,
  LBRACE,
  RBRACE,
  END
};

class Token
{
public:
  explicit Token(std::size_t line);
  Token(e_type type, const std::string& value, std::size_t line);

  e_type getType() const;
  const std::string& getValue() const;
  std::size_t getLine() const;

  void setType(e_type type);
  void setValue(const std::string& value);
  void setLine(std::size_t line);

private:
  e_type _type;
  std::string _value;
  std::size_t _line;
};

std::ostream& operator<<(std::ostream& out, const Token& token);

#endif
