#include "config/Config.hpp"
#include "config/ConfigParser.hpp"
#include "config/ConfigTestSetup.hpp"
#include "config/ServerConfig.hpp"
#include "server/ServerManager.hpp"
#include <config/Config.hpp>
#include <config/ConfigTestSetup.hpp>
#include <config/ServerConfig.hpp>
#include <server/ServerManager.hpp>
#include <utils/logger/Logger.hpp>

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

    Logger::getInstance(LOG_GENERAL).info() << "webserv started\n";

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    ConfigParser parser(argv[1]);
    const Config config = parser.parseConfig();
    ServerManager serverManager(config);
    serverManager.run();

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
