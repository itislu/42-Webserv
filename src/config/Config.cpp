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
const char* const Config::defaultRoot = "/var/www";
const std::size_t Config::defaultMaxBodySize = 1024L * 1024;
const int Config::defaultTimeout = 60;

Config::Config()
  : _root(defaultRoot)
  , _maxBodySize(defaultMaxBodySize)
  , _timeout(defaultTimeout)
{
}

Config& Config::getConfig()
{
  static Config config;

  return config;
}

/* Use this for tests to reset the config back to default and not carry over
 * values from previous test */
void Config::reset()
{
  getConfig() = Config();
}

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

long Config::getTimeout() const
{
  return _timeout;
}

const std::string& Config::getErrorPage(int code) const
{
  const std::map<int, std::string>::const_iterator iter =
    _errorPages.find(code);
  if (iter != _errorPages.end()) {
    return iter->second;
  }
  static const std::string empty;
  return empty;
}

const std::map<int, std::string>& Config::getErrorPages() const
{
  return _errorPages;
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

void Config::setTimeout(long time)
{
  _timeout = time;
}

void Config::setDefaultTimeout()
{
  long timeout = _timeout;

  for (const_ServConfIter it = _servers.begin(); it != _servers.end(); ++it) {
    timeout = std::min(timeout, it->getTimeout());
  }
  timeout = std::min(timeout, static_cast<long>(INT_MAX));
  timeout = std::max(timeout, 0L);

  Config::_defaultTimeout = static_cast<int>(timeout);
}

void Config::setErrorPages(const std::vector<int>& codes,
                           const std::string& path)
{
  for (std::size_t i = 0; i < codes.size(); ++i) {
    addErrorPage(codes[i], path);
  }
}

void Config::addErrorPage(int code, const std::string& path)
{
  _errorPages[code] = path;
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
  out << "Timeout: " << config.getTimeout() << "s\n";
  out << "BodySize: " << config.getMaxBodySize() << "\n";
  if (!config.getErrorPages().empty()) {
    out << "Error Page:" << "\n";
    for (std::map<int, std::string>::const_iterator it =
           config.getErrorPages().begin();
         it != config.getErrorPages().end();
         ++it) {
      out << "  " << it->first << " : " << it->second << "\n";
    }
  }

  const std::vector<ServerConfig>& servers = config.getServers();
  if (servers.empty()) {
    out << "no servers";
    return out;
  }
  for (Config::const_ServConfIter serverIt = servers.begin();
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
      out << "  Location: " << locIt->getPath() << "\n"
          << "    root: " << locIt->getRoot() << "\n"
          << "    autoindex: " << (locIt->isAutoIndex() ? "on" : "off") << "\n"
          << "    cgi: " << (locIt->isCgi() ? "on" : "off") << "\n";
    }
  }
  out << "Lowest Timeout(Default): " << Config::getDefaultTimeout() << "s\n";
  return out;
}
