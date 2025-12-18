#include "event/EventManager.hpp"
#include "socket/SocketManager.hpp"
#include "utils/process/ChildProcessManager.hpp"
#include <config/Config.hpp>
#include <config/ServerConfig.hpp>
#include <config/parser/ConfigParser.hpp>
#include <server/ServerManager.hpp>
#include <utils/logger/Logger.hpp>

#include <cstdlib>
#include <exception>
#include <iostream>

static void initSingletons();

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

  initSingletons();

  logger.info() << "starting server...\n";
  ServerManager::run();

  logger.info() << "server stopped\n";
  return EXIT_SUCCESS;
} catch (const std::exception& e) {
  std::cerr << "Error: " << e.what() << '\n';
  return EXIT_FAILURE;
}
// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)

static void initSingletons()
{
  ChildProcessManager::getInstance();
  SocketManager::getInstance();
  ServerManager::getInstance();
  EventManager::getInstance();
}
