#include <config/Config.hpp>
#include <config/ServerConfig.hpp>
#include <config/parser/ConfigParser.hpp>
#include <server/ServerManager.hpp>
#include <utils/logger/Logger.hpp>

#include <exception>
#include <iostream>

int main(int argc, char* argv[])
{
  Logger::disableLogging();

  if (argc != 2) {
    std::cerr << "Error: Usage: ./webserv [configuration file]\n";
    return 1;
  }

  try {
    Logger::getInstance(LOG_GENERAL).info() << "webserv started\n";

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    config::ConfigParser(argv[1]).parseConfig();
    std::cout << Config::getConfig();

    ServerManager::getInstance();
    ServerManager::run();

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
