#ifndef RESOURCE_HPP
#define RESOURCE_HPP

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

  void setPath(const std::string& path);
  void setType(Type type);

  std::string toString();

private:
  std::string _typeToString();

  Type _type;
  std::string _path;
};

#endif
