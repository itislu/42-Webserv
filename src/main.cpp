#include "config/Config.hpp"
#include "config/ConfigTestSetup.hpp"
#include "config/ServerConfig.hpp"
#include "server/ServerHandler.hpp"
#include <exception>
#include <iostream>

int main(int argc, char* argv[])
{
  (void)argv;
  if (argc != 2) {
    std::cerr << "Error: Usage: ./webserv [configuration file]\n";
    return 1;
  }

  try {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    // const std::string configPath = argv[1]; //
    // const Config config(configPath);

    const Config config = TestConfigSetup::testConfig();

    const ServerHandler serverHandler(config);
    // server.initServer();
    // server.run();

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
