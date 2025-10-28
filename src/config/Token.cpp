#include "Token.hpp"
#include <cstddef>
#include <ostream>
#include <string>

Token::Token(std::size_t line)
  : _type(INVALID)
  , _line(line)
{
}

Token::Token(e_type type, const std::string& value, std::size_t line)
  : _type(type)
  , _value(value)
  , _line(line)
{
}

e_type Token::getType() const
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
  out << "Line: " << token.getLine() << "\n";
  out << "Value: " << token.getValue() << "\n";
  out << "Type: ";
  switch (token.getType()) {
    case INVALID:
      out << "Invalid" << "\n";
      break;
    case IDENT:
      out << "Identifier" << "\n";
      break;
    case SEMICOLON:
      out << "Semicolon (;)" << "\n";
      break;
    case LBRACE:
      out << "Left Brace ({)" << "\n";
      break;
    case RBRACE:
      out << "Right Brace (})" << "\n";
      break;
    case END:
      out << "END" << "\n";
      break;
    default:
      out << "Error: unknown type\n";
      break;
  }
  return out;
}
