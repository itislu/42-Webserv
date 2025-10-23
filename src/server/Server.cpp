#include "Server.hpp"
#include "config/ServerConfig.hpp"
#include "socket/Socket.hpp"
#include <string>
#include <vector>

Server::Server(const ServerConfig* config,
               const std::vector<const Socket*>& listeners)
  : _config(config)
  , _listeners(listeners)
  , _hostnames(config->getHostnames())
  , _timeOut(config->getTimeOut())
{
}

const ServerConfig* Server::getConfig() const
{
  return _config;
}

const std::vector<const Socket*>& Server::getListeners() const
{
  return _listeners;
}

const std::vector<std::string>& Server::getHostnames() const
{
  return _hostnames;
}

long Server::getTimeout() const
{
  return _timeOut;
}
