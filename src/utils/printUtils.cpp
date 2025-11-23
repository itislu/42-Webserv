#include "printUtils.hpp"

#include <iostream>

void printEscapedChar(char chr)
{
  switch (chr) {
    case '\n':
      std::cout << "\\n";
      break;
    case '\r':
      std::cout << "\\r";
      break;
    case '\t':
      std::cout << "\\t";
      break;
    case '\\':
      std::cout << "\\\\";
      break;
    case '\'':
      std::cout << "\\'";
      break;

    default:
      const int controlCharsStart = 32;
      if (static_cast<unsigned char>(chr) < controlCharsStart) {
        // Print other control chars as hex
        std::cout << "\\x" << std::hex << std::uppercase
                  << int(static_cast<unsigned char>(chr)) << std::dec;
      } else {
        std::cout << chr;
      }
  }
}
