#include "config/Config.hpp"
#include "config/ConfigTestSetup.hpp"
#include "config/ServerConfig.hpp"
#include "server/ServerManager.hpp"
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
    const Config config = TestConfigSetup::createTestConfig();
    ServerManager serverManager(&config);

    serverManager.run();

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
