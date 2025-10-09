#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "config/LocationConfig.hpp"
#include <cstddef>
#include <map>
#include <string>
#include <vector>

class Config;

class ServerConfig
{
public:
  // Constructor initializes with defaults
  explicit ServerConfig(const Config& global);

  // GETTERS
  const std::vector<int>& getPorts() const;
  const std::vector<std::string>& getHostnames() const;
  const std::string& getRoot() const;
  const std::string& getIndex() const;
  const std::map<int, std::string>& getErrorPages() const;
  std::size_t getMaxBodySize() const;
  const std::vector<std::string>& getAllowedMethods() const;
  std::size_t getTimeOut() const;
  const std::vector<LocationConfig>& getLocations() const;

  // SETTERS
  void addPort(int port);
  void addHostName(const std::string& hostName);
  void setRoot(const std::string& root);
  void setIndex(const std::string& index);
  void addErrorPage(int code, const std::string& path);
  void setMaxBodySize(std::size_t size);
  void addAllowedMethod(const std::string& method);
  void setTimeOut(std::size_t time);

  const LocationConfig& getLocationForPath(const std::string& uri) const;

private:
  std::vector<int> _ports;             // listeners
  std::vector<std::string> _hostnames; // names for virtual hosting?
                                       //    - same port but different hostname
  std::string _root;                   // root dir
  std::string _index;                  // index.html
  std::map<int, std::string> _errorPages;   // map for different Error codes
  std::size_t _maxBodySize;                 // limit upload size (POST)
  std::vector<std::string> _allowedMethods; // usually per location
  std::size_t _timeOut;                     // timeout

  std::vector<LocationConfig> _locations;
};

#endif