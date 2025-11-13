#include "ServerConfig.hpp"
#include "Config.hpp"
#include "LocationConfig.hpp"
#include <cstddef>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
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

const std::set<std::string>& ServerConfig::getAllowedMethods() const
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
  checkPortDuplicate(port);
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

void ServerConfig::setErrorPages(std::vector<int> codes,
                                 const std::string& path)
{
  for (std::size_t i = 0; i < codes.size(); ++i) {
    addErrorPage(codes[i], path);
  }
}

void ServerConfig::addErrorPage(int code, const std::string& path)
{
  _errorPages[code] = path;
}

void ServerConfig::setMaxBodySize(std::size_t size)
{
  _maxBodySize = size;
}

void ServerConfig::setAllowedMethod(const std::string& method)
{
  _allowedMethods.insert(method);
}

void ServerConfig::setTimeout(long time)
{
  _timeOut = time;
}

void ServerConfig::addLocation(const LocationConfig& location)
{
  _locations.push_back(location);
}

void ServerConfig::checkPortDuplicate(int port)
{
  for (std::vector<int>::const_iterator it = _ports.begin(); it != _ports.end();
       ++it) {
    if (port == *it) {
      std::ostringstream oss;
      oss << port;
      throw std::invalid_argument("duplicated port " + oss.str());
    }
  }
}

/* const LocationConfig& ServerConfig::getLocationForPath(const std::string&
uri) const
{

}
 */
