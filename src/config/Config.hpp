
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
  explicit Config();
  typedef std::vector<ServerConfig>::iterator ServConfIter;
  typedef std::vector<ServerConfig>::const_iterator const_ServConfIter;

  // Getters
  const std::vector<ServerConfig>& getServers() const;

  const std::string& getRoot() const;
  std::size_t getTimeout() const;
  std::size_t getMaxBodySize() const;
  const std::string& getErrorPage(int code) const;
  const std::map<int, std::string>& getErrorPages() const;

  // Setters
  void addServer(const ServerConfig& server);

  void setRoot(const std::string& root);
  void setTimeout(std::size_t seconds);
  void setMaxBodySize(std::size_t bytes);
  void setErrorPages(std::vector<int> codes, const std::string& path);
  void addErrorPage(int code, const std::string& path);

  // Default Timeout
  static int getDefaultTimeout();
  void setDefaultTimeout();

  const ServerConfig* getServerForRequest(const std::string& host,
                                          int port) const;
  const LocationConfig* getLocationForPath(const ServerConfig& server,
                                           const std::string& path) const;

private:
  static const char* const DEFAULT_ROOT;
  static const std::size_t DEFAULT_BODY_SIZE;
  static const int DEFAULT_TIMEOUT;

  std::string _root;
  std::size_t _maxBodySize;
  std::map<int, std::string> _errorPages;
  std::vector<ServerConfig> _servers;
  static int _defaultTimeout;
  std::size_t _timeout;
};

std::ostream& operator<<(std::ostream& out, const Config& config);

#endif
