#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "config/LocationConfig.hpp"
#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace config {

class Config;

class ServerConfig
{
public:
  explicit ServerConfig(const Config& global);

  // GETTERS
  const std::vector<LocationConfig>& getLocations() const;

  const std::vector<int>& getPorts() const;
  const std::vector<std::string>& getHostnames() const;

  const std::string& getRoot() const;
  const std::string& getIndex() const;
  const std::map<int, std::string>& getErrorPages() const;
  std::size_t getMaxBodySize() const;
  const std::set<std::string>& getAllowedMethods() const;
  long getTimeout() const;

  // SETTERS
  void addLocation(const LocationConfig& location);

  void addPort(int port);
  void addHostName(const std::string& hostName);

  void setRoot(const std::string& root);
  void setIndex(const std::string& index);
  void setTimeout(long time);
  void setMaxBodySize(std::size_t size);
  void setErrorPages(const std::vector<int>& codes, const std::string& path);
  void addErrorPage(int code, const std::string& path);
  void addAllowedMethod(const std::string& method);

  const LocationConfig* getBestMatchLocation(const std::string& uri) const;
  static std::size_t getMatchLength(const std::string& uri,
                                    const std::string& path);

private:
  void checkPortDuplicate(int port);

  std::vector<int> _ports;             // listeners
  std::vector<std::string> _hostnames; // names for virtual hosting?
                                       //    - same port but different hostname
  std::string _root;                   // root dir
  std::string _index;                  // index.html
  std::map<int, std::string> _errorPages; // map for different Error codes
  std::size_t _maxBodySize;               // limit upload size (POST)
  std::set<std::string> _allowedMethods;  // usually per location
  long _timeOut;                          // timeout

  std::vector<LocationConfig> _locations;
};

} // namespace config

#endif
