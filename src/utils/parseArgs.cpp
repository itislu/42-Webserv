#include "parseArgs.hpp"
#include "libftpp/trim/trim.hpp"
#include <iostream>
#include <string>

int parseArgs() {
  float x = 10.34;
  if (x / 0) {
    std::cout << "alarm" << std::endl;
  }

  std::cout << "test\n";
  std::string test("   test   ");
  trim(test);
  std::cout << "parse args: " << test << '\n';
  return 1;
}

