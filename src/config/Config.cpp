#include "Config.hpp"
#include "ServerConfig.hpp"
#include "config/LocationConfig.hpp"
#include <cstddef>
#include <map>
#include <ostream>
#include <string>
#include <vector>

Config::Config(const std::string& configFile)
  : _configFile(configFile)
  , _defaultMaxBodySize()
  , _defaultTimeOut()
{
}

const std::vector<ServerConfig>& Config::getServers() const
{
  return _servers;
}

size_t Config::getDefaultBodySize() const
{
  return _defaultMaxBodySize;
}

std::size_t Config::getDefaultTimeout() const
{
  return _defaultTimeOut;
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
  return _accesLogPath;
}

void Config::addServer(const ServerConfig& server)
{
  _servers.push_back(server);
}

void Config::setDefaultMaxBodySize(std::size_t bytes)
{
  _defaultMaxBodySize = bytes;
}

void Config::setDefaultTimeout(std::size_t seconds)
{
  _defaultTimeOut = seconds;
}

void Config::setErrorLogPath(const std::string& path)
{
  _errorLogPath = path;
}

void Config::setAccessLogPath(const std::string& path)
{
  _accesLogPath = path;
}

/* const ServerConfig* Config::getServerForRequest(const std::string& host,
                                                int port) const
{
  // TODO: Implement
}
const LocationConfig* Config::getLocationForPath(const ServerConfig& server,
                                                 const std::string& path) const
{
  // TODO: Implement
}
 */

std::ostream& operator<<(std::ostream& out, const Config& config)
{
  const std::vector<ServerConfig>& servers = config.getServers();
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
    out << "Timeout: " << serverIt->getTimeOut() << "s\n";
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
  return out;
}
