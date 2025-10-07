#include "Config.hpp"
#include <string>
#include <vector>

Config::Config(const std::string& file)
  : _maxClients(MAX_CLIENTS)
{
  // parse file
  (void)file;
  const int port1 = 8080;
  const int port2 = 8081;

  _ports.push_back(port1);
  _ports.push_back(port2);
}

const std::vector<int>& Config::getPorts() const
{
  return _ports;
}
