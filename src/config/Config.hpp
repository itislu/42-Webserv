
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "LocationConfig.hpp"
#include "ServerConfig.hpp"
#include <cstddef>
#include <map>
#include <ostream>
#include <string>
#include <vector>

class Config
{
public:
  typedef std::vector<ServerConfig> ServerConfigs;
  typedef ServerConfigs::iterator ServConfIter;
  typedef ServerConfigs::const_iterator const_ServConfIter;

  explicit Config(const std::string& configFile);

  // Getters
  const ServerConfigs& getServers() const;
  std::size_t getMaxBodySize() const;
  long getTimeout() const;
  const std::string& getErrorLogPath() const;
  const std::string& getAccessLogPath() const;
  const std::map<int, std::string>& getErrorPages() const;

  // Setters
  void addServer(const ServerConfig& server);
  void setMaxBodySize(std::size_t bytes);
  void setTimeout(long seconds);
  void setErrorLogPath(const std::string& path);
  void setAccessLogPath(const std::string& path);
  void setDefaultTimeout();

  static int getDefaultTimeout();

  const ServerConfig* getServerForRequest(const std::string& host,
                                          int port) const;
  const LocationConfig* getLocationForPath(const ServerConfig& server,
                                           const std::string& path) const;

private:
  std::string _configFile;
  std::string _root;
  std::size_t _maxBodySize;
  std::map<int, std::string> _errorPages;
  ServerConfigs _servers;
  static int _defaultTimeout;
  long _timeout;

  // for Logging
  std::string _errorLogPath;
  std::string _accessLogPath;
};

std::ostream& operator<<(std::ostream& out, const Config& config);

#endif
