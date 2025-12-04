#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <cstddef>
#include <ostream>
#include <string>

namespace config {

class Token
{
public:
  enum Type
  {
    Invalid,
    Ident,
    Semicolon,
    LBrace,
    RBrace,
    Comment,
    End
  };

  explicit Token(std::size_t line);
  Token(Type type, const std::string& value, std::size_t line);

  Type getType() const;
  const std::string& getValue() const;
  std::size_t getLine() const;

  void setType(Type type);
  void setValue(const std::string& value);
  void setLine(std::size_t line);

private:
  Type _type;
  std::string _value;
  std::size_t _line;
};

std::ostream& operator<<(std::ostream& out, const Token& token);

} // namespace config

#endif
