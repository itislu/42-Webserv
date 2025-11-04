
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
  typedef std::vector<ServerConfig>::iterator servConfIt;
  typedef std::vector<ServerConfig>::const_iterator const_servConfIt;

public:
  // Getters
  const std::vector<ServerConfig>& getServers() const;
  std::size_t getMaxBodySize() const;
  std::size_t getTimeout() const;
  const std::string& getRoot() const;
  const std::string& getErrorLogPath() const;
  const std::string& getAccessLogPath() const;
  const std::map<int, std::string>& getErrorPages() const;

  // Setters
  void addServer(const ServerConfig& server);
  void setRoot(const std::string& root);
  void setMaxBodySize(std::size_t bytes);
  void setTimeout(std::size_t seconds);
  void setErrorLogPath(const std::string& path);
  void setAccessLogPath(const std::string& path);
  void setErrorPages(std::map<int, std::string>& errorPages);
  void setDefaultTimeout();

  static int getDefaultTimeout();

  const ServerConfig* getServerForRequest(const std::string& host,
                                          int port) const;
  const LocationConfig* getLocationForPath(const ServerConfig& server,
                                           const std::string& path) const;

private:
  std::string _root;
  std::size_t _maxBodySize;
  std::map<int, std::string> _errorPages;
  std::vector<ServerConfig> _servers;
  static int _defaultTimeout;
  std::size_t _timeout;

  // for Logging
  std::string _errorLogPath;
  std::string _accessLogPath;
};

std::ostream& operator<<(std::ostream& out, const Config& config);

#endif
