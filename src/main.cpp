#include "config/Config.hpp"
#include "server/Server.hpp"
#include <exception>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
  (void)argv;
  if (argc != 2) {
    std::cerr << "Error: Usage: ./webserv [configuration file]\n";
    return 1;
  }

  try {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    const std::string configPath = argv[1]; //
    const Config config(configPath);

    Server server(config);
    server.initServer();
    server.run();

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
