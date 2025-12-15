#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <config/LocationConfig.hpp>
#include <config/ServerConfig.hpp>
#include <libftpp/optional.hpp>

#include <string>

class Resource
{
public:
  enum Type
  {
    File,
    Autoindex,
    Cgi,
    Error,
    Undefined
  };

  Resource();
  Type getType() const;
  const std::string& getPath() const;
  const std::string& getNoRootPath() const;
  const LocationConfig* getLocation();
  const ServerConfig* getServer();

  void setPath(const std::string& path);
  void setNoRootPath(const std::string& noRootPath);
  void setType(Type type);
  void setLocation(const LocationConfig* location);
  void setServer(const ServerConfig* server);

  ft::optional<std::string> getErrorPage(int code) const;

  std::string toString();

private:
  std::string _typeToString();

  Type _type;
  std::string _path;
  std::string _noRootPath;
  const LocationConfig* _location;
  const ServerConfig* _server;
};

#endif
