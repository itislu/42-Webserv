#ifndef SERVERBUILDER_HPP
#define SERVERBUILDER_HPP

#include "config/ParsedServer.hpp"
#include "config/ServerConfig.hpp"

class ServerBuilder
{
public:
  static ServerConfig build(const ParsedServer& parsed, const Config& config);

private:
};

#endif
