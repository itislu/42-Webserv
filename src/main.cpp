#include "server/Server.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
  (void)argv;
  if (argc != 2) {
    std::cerr << "Error: Usage: ./webserv [configuration file]\n";
    return 1;
  }

  Server server(8080);
  server.run();

  return 0;
}
// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)
