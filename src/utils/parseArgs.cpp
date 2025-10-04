#include "parseArgs.hpp"
#include "libftpp/string.hpp"
#include <iostream>
#include <string>

int parseArgs()
{
  std::cout << "test\n";
  std::string test("   test   ");
  ft::trim(test);
  std::cout << "parse args: " << test << '\n';
  return 1;
}
