#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
class ConfigParser
{
public:
  explicit ConfigParser(const char* path);

  void readConfig();

private:
  void validateInputFile() const;
  void parse();

  std::string _filepath;
};

#endif
