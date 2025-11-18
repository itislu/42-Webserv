#ifndef SERVER_HPP
#define SERVER_HPP

#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include "socket/Socket.hpp"
#include <string>
#include <vector>

class Server
{
public:
  Server(const config::ServerConfig& servConfig,
         const std::vector<const Socket*>& listeners);

  const config::ServerConfig& getConfig() const;
  const std::vector<const Socket*>& getListeners() const;
  const std::vector<std::string>& getHostnames() const;
  long getTimeout() const;

private:
  const config::ServerConfig* _config;

  // server specific
  std::vector<const Socket*> _listeners; // listeners (ports)
  std::vector<std::string> _hostnames;   // names for virtual hosting
  long _timeOut;

  // Maybe for Logging
  // std::string _errorLogPath;
  // std::string _accesLogPath;
};

#endif
