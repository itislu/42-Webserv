#ifndef SERVER_HPP
#define SERVER_HPP

#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include "socket/Socket.hpp"
#include <string>
#include <vector>

// maybe just store a pointer to the ServerConfig here?
class Server
{
public:
  Server(const ServerConfig& servConfig,
         const std::vector<const Socket*>& listeners);

  const ServerConfig& getConfig() const;
  const std::vector<const Socket*>& getListeners() const;
  const std::vector<std::string>& getHostnames() const;
  long getTimeout() const;

private:
  const ServerConfig* _config;

  // server specific
  std::vector<const Socket*> _listeners; // listeners (ports)
  std::vector<std::string> _hostnames;   // names for virtual hosting
  long _timeOut;

  // Maybe for Logging
  // std::string _errorLogPath;
  // std::string _accesLogPath;
};

#endif
