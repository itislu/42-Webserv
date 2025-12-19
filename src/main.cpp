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

const char* const defaultConfigPath = "TODO.conf"; // TODO

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic): argv.
int main(int argc, char* argv[])
try {
  Logger& logger = Logger::getInstance(LOG_GENERAL);

  if (argc > 2) {
    std::cerr << "Usage: " << argv[0] << " [configuration file]\n";
    return EXIT_FAILURE;
  }

  const char* const configPath = (argc == 2) ? argv[1] : defaultConfigPath;
  logger.info() << "parsing config in \"" << configPath << "\"...\n";
  config::ConfigParser(configPath).parseConfig();
  std::cout << Config::getConfig();

  initSingletons();

  logger.info() << "starting server...\n";
  ServerManager::run();

  logger.info() << "server stopped\n";
  return EXIT_SUCCESS;
} catch (const std::exception& e) {
  std::cerr << "Error: " << e.what() << '\n';
  return EXIT_FAILURE;
} catch (...) {
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
