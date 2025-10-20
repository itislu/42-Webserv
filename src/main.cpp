#include "config/Config.hpp"
#include "config/ConfigTestSetup.hpp"
#include "config/ServerConfig.hpp"
#include "server/Server.hpp"
#include "server/ServerHandler.hpp"
#include "server/ServerManager.hpp"
#include "socket/SocketManager.hpp"
#include "client/ClientManager.hpp"
#include "event/EventManager.hpp"
#include <exception>
#include <iostream>

int main() {
  Config config = TestConfigSetup::createTestConfig();
  ServerManager serverManager(config);

  serverManager.run();

  return 0;
}

/* int main(int argc, char* argv[])
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

    const Config config = TestConfigSetup::createTestConfig();
    std::cout << config;
    ServerHandler serverHandler(config);
    serverHandler.debugPrintMaps();
    serverHandler.run();

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
 */