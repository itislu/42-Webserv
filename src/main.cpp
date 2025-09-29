#include "libftpp/trim/trim.hpp"
#include "utils/parseArgs.hpp"
#include <iostream>
#include <ostream>
#include <string>

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "invalid arguments" << std::endl;
    return 1;
  }
  if (argc >= 2) {
    parseArgs();
    std::string str(argv[1]);
    std::cout << trim(str) << "\n";
  }
  return 0;
}
