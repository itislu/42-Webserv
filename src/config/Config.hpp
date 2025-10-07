
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>

#ifndef MAX_CLIENTS
#define MAX_CLIENTS 1024
#endif

class Config
{
public:
  explicit Config(const std::string& file);

  const std::vector<int>& getPorts() const;
  int getMaxClients() const;

private:
  std::vector<int> _ports;
  int _maxClients;
};

#endif
