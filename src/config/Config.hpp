
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>

class Config
{
public:
  explicit Config(const std::string& file);

  const std::vector<int>& getPorts() const;

private:
  std::vector<int> _ports;
};

#endif