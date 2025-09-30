#include <iostream>
#include <ostream>
#include <string>
#include "libftpp/trim/trim.hpp"

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "invalid arguments" << std::endl;
    return 1;
  }
  if (argc >= 2) {
    std::string str(argv[1]);
    std::cout << trim(str) << "\n";
  }
  return 0; //hello
}
