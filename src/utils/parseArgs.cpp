#include "parseArgs.hpp"
#include "libftpp/trim/trim.hpp"
#include <iostream>
#include <string>

int parseArgs() {
  std::cout << "test\n";
  std::string test("   test   ");
  trim(test);
  std::cout << "parse args: " << test << '\n';
  return 1;
}

