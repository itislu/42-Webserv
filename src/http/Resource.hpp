#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>

class Resource
{
  enum Type
  {
    File,
    Autoindex,
    Cgi,
    Error
  };

public:
  Type getType() const;
  const std::string& getPath() const;

  void setPath(const std::string& path);
  void setType(Type type);

private:
  Type _type;
  std::string _path;
};

#endif
