#ifndef PARSEDLOCATION_HPP
#define PARSEDLOCATION_HPP

#include <map>
#include <string>
#include <vector>

class ParsedLocation
{
public:
  explicit ParsedLocation(const std::string& path);

  void setPath(const std::string& path);

  std::map<std::string, std::vector<std::string> >& getDirective();
  const std::map<std::string, std::vector<std::string> >& getDirective() const;
  const std::string& getPath() const;

private:
  std::string _path;
  std::map<std::string, std::vector<std::string> > _directives;
};

#endif
