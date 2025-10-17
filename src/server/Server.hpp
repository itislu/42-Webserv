#ifndef SERVER_HPP
#define SERVER_HPP

#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include "socket/Socket.hpp"
#include <cstddef>
#include <map>
#include <string>
#include <vector>

// maybe just store a pointer to the ServerConfig here?
class Server
{
public:
  Server(const ServerConfig& servConfig,
         const std::vector<const Socket*>& listeners);

  const std::vector<const Socket*>& getListeners() const;
  const std::vector<std::string>& getHostnames() const;
  long getTimeout() const;

private:
  // server specific
  std::vector<const Socket*> _listeners; // listeners (ports)
  std::vector<std::string> _hostnames;   // names for virtual hosting

  // can be defaulted
  std::string _root;
  std::string _index; // index.html
  std::map<int, std::string> _errorPages;
  std::size_t _maxBodySize;
  std::vector<std::string> _allowedMethods; // usually per location
  long _timeOut;
  std::vector<LocationConfig> _locations;

  // Maybe for Logging
  // std::string _errorLogPath;
  // std::string _accesLogPath;
};

#endif
