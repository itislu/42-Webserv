#include "Config.hpp"
#include "ServerConfig.hpp"
#include <cstddef>
#include <map>
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
