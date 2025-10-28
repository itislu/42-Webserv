#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
class ConfigParser
{
public:
  explicit ConfigParser(const char* path);

  void parse();

private:
  bool validateInputFile() const;
  bool checkFileExtension() const;
  bool isFile() const;

  std::string _filepath;
};

#endif
