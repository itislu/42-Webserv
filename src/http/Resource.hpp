#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <config/LocationConfig.hpp>
#include <config/ServerConfig.hpp>
#include <libftpp/optional.hpp>

#include <cstddef>
#include <string>

class Resource
{
public:
  enum Type
  {
    Undefined,
    File,
    Autoindex,
    Cgi,
    Error
  };

  Resource();
  Type getType() const;
  const std::string& getHost() const;
  const std::string& getPath() const;
  const std::string& getPathInfo() const;
  const std::string& getNoRootPath() const;
  const LocationConfig* getLocation() const;
  const ServerConfig* getServer() const;
  int getPort() const;

  void setHost(const std::string& host);
  void setPath(const std::string& path);
  void setNoRootPath(const std::string& noRootPath);
  void setType(Type type);
  void setLocation(const LocationConfig* location);
  void setServer(const ServerConfig* server);
  void setPort(int port);
  void setPathInfo(const std::string& pathInfo);

  ft::optional<std::string> getErrorPage(int code) const;
  std::size_t getMaxBodySize() const;

  std::string toString() const;

private:
  std::string _typeToString() const;

  Type _type;
  std::string _host;
  std::string _path;
  std::string _pathInfo;
  std::string _noRootPath;
  const LocationConfig* _location;
  const ServerConfig* _server;
  int _port;
};

#endif
