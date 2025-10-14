#include "Server.hpp"
#include "config/ServerConfig.hpp"
#include "socket/Socket.hpp"
#include <string>
#include <vector>

Server::Server(const ServerConfig& config,
               const std::vector<Socket const*>& listeners)
  : _listeners(listeners)
  , _hostnames(config.getHostnames())
  , _root(config.getRoot())
  , _index(config.getIndex())
  , _errorPages(config.getErrorPages())
  , _maxBodySize(config.getMaxBodySize())
  , _allowedMethods(config.getAllowedMethods())
  , _timeOut(config.getTimeOut())
  , _locations(config.getLocations())
{
}

const std::vector<Socket const*>& Server::getListeners() const
{
  return _listeners;
}

const std::vector<std::string>& Server::getHostnames() const
{
  return _hostnames;
}
