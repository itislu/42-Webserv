#include "server/Server.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
  (void)argv;
  if (argc != 2) {
    std::cerr << "Error: Usage: ./webserv [configuration file]\n";
    return 1;
  }

  const int port = 8080;
  Server server(port);
  server.run();

  return 0;
}
