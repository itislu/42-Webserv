#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include <string>

class Resource
{
public:
  enum Type
  {
    File,
    Autoindex,
    Cgi,
    Error
  };

  Type getType() const;
  const std::string& getPath() const;
  const config::LocationConfig* getLocation();
  const config::ServerConfig* getServer();

  void setPath(const std::string& path);
  void setType(Type type);
  void setLocation(const config::LocationConfig* location);
  void setServer(const config::ServerConfig* server);

  std::string toString();

private:
  std::string _typeToString();

  Type _type;
  std::string _path;
  const config::LocationConfig* _location;
  const config::ServerConfig* _server;
};

#endif
