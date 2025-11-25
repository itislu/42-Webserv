#include "ServerConfig.hpp"
#include "Config.hpp"
#include "LocationConfig.hpp"
#include "libftpp/utility.hpp"
#include <cstddef>
#include <libftpp/algorithm.hpp>
#include <libftpp/string.hpp>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace config {

ServerConfig::ServerConfig(const Config& global)
  : _index("index.html")
  , _errorPages(global.getErrorPages())
  , _maxBodySize(global.getMaxBodySize())
  , _timeOut(global.getTimeout())
{
  _allowedMethods.insert("GET");
  _allowedMethods.insert("POST");
  _allowedMethods.insert("DELETE");
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

const std::string& ServerConfig::getErrorPage(int code) const
{
  const std::map<int, std::string>::const_iterator iter =
    _errorPages.find(code);
  if (iter != _errorPages.end()) {
    return iter->second;
  }
  static const std::string empty;
  return empty;
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

void ServerConfig::setErrorPages(const std::vector<int>& codes,
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

void ServerConfig::addAllowedMethod(const std::string& method)
{
  _allowedMethods.insert(method);
}

void ServerConfig::clearAllowedMethods()
{
  _allowedMethods.clear();
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
  if (ft::contains(_ports, port)) {
    throw std::invalid_argument("duplicate port " + ft::to_string(port));
  }
}

const LocationConfig* ServerConfig::getBestMatchLocation(
  const std::string& uri) const
{
  std::size_t bestMatchLen = 0;
  const LocationConfig* bestMatch = FT_NULLPTR;

  const std::vector<LocationConfig>& locations = getLocations();
  for (std::vector<LocationConfig>::const_iterator it = locations.begin();
       it != locations.end();
       ++it) {
    if (uri.rfind(it->getPath(), 0) == 0) {
      if (it->getPath().length() > bestMatchLen) {
        bestMatch = &*it;
        bestMatchLen = it->getPath().length();
      }
    }
  }
  return bestMatch;
}

} // namespace config
