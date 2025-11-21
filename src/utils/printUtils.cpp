#include "printUtils.hpp"

#include <iostream>

std::ostream& printEscapedChar(std::ostream& out, char chr)
{
  switch (chr) {
    case '\n':
      out << "\\n";
      break;
    case '\r':
      out << "\\r";
      break;
    case '\t':
      out << "\\t";
      break;
    case '\\':
      out << "\\\\";
      break;
    case '\'':
      out << "\\'";
      break;

    default:
      const int otherControlChars = 32;
      if (static_cast<unsigned char>(chr) < otherControlChars) {
        // Print other control chars as hex
        out << "\\x" << std::hex << std::uppercase
            << int(static_cast<unsigned char>(chr)) << std::dec;
      } else {
        out << chr;
      }
  }
  return out;
}
