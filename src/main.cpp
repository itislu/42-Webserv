#include "config/Config.hpp"
#include "config/ConfigParser.hpp"
#include "config/ServerConfig.hpp"
#include "server/ServerManager.hpp"
#include <utils/logger/Logger.hpp>

#include <exception>
#include <iostream>

int main(int argc, char* argv[])
{
  Logger::enableLogging();

  if (argc != 2) {
    std::cerr << "Error: Usage: ./webserv [configuration file]\n";
    return 1;
  }

  try {
    Logger::getInstance(LOG_GENERAL).info() << "webserv started\n";

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    config::ConfigParser(argv[1]).parseConfig();
    std::cout << config::Config::getInstance();

    return 0;

    // ServerManager& serverManager = ServerManager::getInstance();
    // serverManager.run();

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
