#include "Config.hpp"
#include "ServerConfig.hpp"
#include "config/LocationConfig.hpp"
#include <algorithm>
#include <climits>
#include <cstddef>
#include <map>
#include <ostream>
#include <string>
#include <vector>

int Config::_defaultTimeout = 0;

// Config::Config()
//   : _maxBodySize()
//   , _timeout()
// {
// }

const std::vector<ServerConfig>& Config::getServers() const
{
  return _servers;
}

const std::string& Config::getRoot() const
{
  return _root;
}

std::size_t Config::getMaxBodySize() const
{
  return _maxBodySize;
}

std::size_t Config::getTimeout() const
{
  return _timeout;
}

const std::map<int, std::string>& Config::getErrorPages() const
{
  return _errorPages;
}

const std::string& Config::getErrorLogPath() const
{
  return _errorLogPath;
}

const std::string& Config::getAccessLogPath() const
{
  return _accessLogPath;
}

void Config::addServer(const ServerConfig& server)
{
  _servers.push_back(server);
}

void Config::setRoot(const std::string& root)
{
  _root = root;
}

void Config::setMaxBodySize(std::size_t bytes)
{
  _maxBodySize = bytes;
}

void Config::setTimeout(std::size_t seconds)
{
  _timeout = seconds;
}

void Config::setErrorLogPath(const std::string& path)
{
  _errorLogPath = path;
}

void Config::setAccessLogPath(const std::string& path)
{
  _accessLogPath = path;
}

void Config::setDefaultTimeout()
{
  std::size_t timeout = _timeout;

  for (const_servConfIt it = _servers.begin(); it != _servers.end(); ++it) {
    timeout = std::min(timeout, it->getTimeout());
  }
  timeout = std::min(timeout, static_cast<size_t>(INT_MAX));
  timeout = std::max(timeout, std::size_t(0));

  Config::_defaultTimeout = static_cast<int>(timeout);
}

void Config::setErrorPages(std::map<int, std::string>& errorPages)
{
  _errorPages = errorPages;
}

int Config::getDefaultTimeout()
{
  return Config::_defaultTimeout;
}

std::ostream& operator<<(std::ostream& out, const Config& config)
{
  out << "==============================" << "\n";
  out << " Global Configuration" << "\n";
  out << "==============================" << "\n";

  out << "Root: " << config.getRoot() << "\n";
  out << "Timeout: " << config.getTimeout() << "\n";
  out << "BodySize: " << config.getMaxBodySize() << "\n";
  out << "Default: " << Config::getDefaultTimeout() << "\n";

  const std::vector<ServerConfig>& servers = config.getServers();
  if (servers.empty()) {
    out << "no servers";
    return out;
  }
  for (std::vector<ServerConfig>::const_iterator serverIt = servers.begin();
       serverIt != servers.end();
       ++serverIt) {
    out << "Server ports: ";

    const std::vector<int>& ports = serverIt->getPorts();
    for (std::vector<int>::const_iterator portIt = ports.begin();
         portIt != ports.end();
         ++portIt) {
      out << *portIt << " ";
    }
    out << "\n";
    out << "Timeout: " << serverIt->getTimeout() << "s\n";
    out << "Hosts: ";
    const std::vector<std::string>& hostnames = serverIt->getHostnames();
    for (std::vector<std::string>::const_iterator hostIt = hostnames.begin();
         hostIt != hostnames.end();
         ++hostIt) {
      out << *hostIt << " ";
    }
    out << "\n";

    const std::vector<LocationConfig>& locations = serverIt->getLocations();
    for (std::vector<LocationConfig>::const_iterator locIt = locations.begin();
         locIt != locations.end();
         ++locIt) {
      out << "  Location: " << locIt->getPath()
          << ", root: " << locIt->getRoot()
          << ", autoindex: " << (locIt->isAutoindex() ? "on" : "off") << "\n";
    }
  }
  out << "Lowest Timeout(Default): " << Config::getDefaultTimeout() << "s\n";
  return out;
}
