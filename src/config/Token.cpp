#include "Token.hpp"
#include <cstddef>
#include <ostream>
#include <string>

namespace config {

Token::Token(std::size_t line)
  : _type(Token::Invalid)
  , _line(line)
{
}

Token::Token(e_type type, const std::string& value, std::size_t line)
  : _type(type)
  , _value(value)
  , _line(line)
{
}

Token::e_type Token::getType() const
{
  return _type;
}
const std::string& Token::getValue() const
{
  return _value;
}

std::size_t Token::getLine() const
{
  return _line;
}

void Token::setType(e_type type)
{
  _type = type;
}

void Token::setValue(const std::string& value)
{
  _value = value;
}

void Token::setLine(std::size_t line)
{
  _line = line;
}

std::ostream& operator<<(std::ostream& out, const Token& token)
{
  out << "Token:\n";
  out << "\tLine: " << token.getLine() << "\n";
  out << "\tValue: " << token.getValue() << "\n";
  out << "\tType: ";
  switch (token.getType()) {
    case Token::Invalid:
      out << "Invalid\n";
      break;
    case Token::Ident:
      out << "Identifier\n";
      break;
    case Token::Semicolon:
      out << "Semicolon (;)\n";
      break;
    case Token::LBrace:
      out << "Left Brace ({)\n";
      break;
    case Token::RBrace:
      out << "Right Brace (})\n";
      break;
    case Token::Comment:
      out << "Comment (#)\n";
      break;
    case Token::End:
      out << "Token::End\n";
      break;
  }
  return out;
}

} // namespace config
