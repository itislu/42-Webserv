#ifndef PARSEDLOCATION_HPP
#define PARSEDLOCATION_HPP

#include "config/ConfigTypes.hpp"
#include <string>

namespace config {


class ParsedLocation
{
public:
  explicit ParsedLocation(const std::string& path);

  void setPath(const std::string& path);

  DirectiveMap& getDirectives();
  const DirectiveMap& getDirectives() const;
  const std::string& getPath() const;

private:
  std::string _path;
  DirectiveMap _directives;
};

} // namespace config


#endif
