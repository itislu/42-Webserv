#include "ServerConfig.hpp"
#include "Config.hpp"
#include "LocationConfig.hpp"
#include <cstddef>
#include <map>
#include <string>
#include <vector>

ServerConfig::ServerConfig(const Config& global)
  : _errorPages(global.getErrorPages())
  , _maxBodySize(global.getMaxBodySize())
  , _timeOut(global.getTimeout())
{
}

// GETTERS
const std::vector<int>& ServerConfig::getPorts() const
{
  return _ports;
}

const std::vector<std::string>& ServerConfig::getHostnames() const
{
  return _hostnames;
}

const std::string& ServerConfig::getRoot() const
{
  return _root;
}

const std::string& ServerConfig::getIndex() const
{
  return _index;
}

const std::map<int, std::string>& ServerConfig::getErrorPages() const
{
  return _errorPages;
}

const std::vector<std::string>& ServerConfig::getAllowedMethods() const
{
  return _allowedMethods;
}

const std::vector<LocationConfig>& ServerConfig::getLocations() const
{
  return _locations;
}

std::size_t ServerConfig::getMaxBodySize() const
{
  return _maxBodySize;
}

long ServerConfig::getTimeout() const
{
  return _timeOut;
}

// SETTERS
void ServerConfig::addPort(int port)
{
  _ports.push_back(port);
}

void ServerConfig::addHostName(const std::string& hostName)
{
  _hostnames.push_back(hostName);
}

void ServerConfig::setRoot(const std::string& root)
{
  _root = root;
}

void ServerConfig::setIndex(const std::string& index)
{
  _index = index;
}

// overrides if errorpage already exists
void ServerConfig::addErrorPage(int code, const std::string& path)
{
  _errorPages[code] = path;
}

void ServerConfig::setMaxBodySize(std::size_t size)
{
  _maxBodySize = size;
}

void ServerConfig::addAllowedMethod(const std::string& method)
{
  _allowedMethods.push_back(method);
}

void ServerConfig::setTimeout(long time)
{
  _timeOut = time;
}

void ServerConfig::addLocation(const LocationConfig& location)
{
  _locations.push_back(location);
}

/* const LocationConfig& ServerConfig::getLocationForPath(const std::string&
uri) const
{

}
 */
