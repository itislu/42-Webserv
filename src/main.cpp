#include <config/Config.hpp>
#include <config/ServerConfig.hpp>
#include <config/parser/ConfigParser.hpp>
#include <server/ServerManager.hpp>
#include <utils/logger/Logger.hpp>

#include <cstdlib>
#include <exception>
#include <iostream>

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic): argv.
int main(int argc, char* argv[])
try {
  Logger& logger = Logger::getInstance(LOG_GENERAL);

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <configuration file>\n";
    return EXIT_FAILURE;
  }

  logger.info() << "parsing config...\n";
  config::ConfigParser(argv[1]).parseConfig();
  std::cout << Config::getConfig();

  logger.info() << "starting server...\n";
  ServerManager::getInstance();
  ServerManager::run();

  logger.info() << "server stoped\n";
  return EXIT_SUCCESS;
} catch (const std::exception& e) {
  std::cerr << "Error: " << e.what() << '\n';
  return EXIT_FAILURE;
}
// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)
