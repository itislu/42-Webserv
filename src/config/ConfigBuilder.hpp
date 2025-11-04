#ifndef CONFIGBUILDER_HPP
#define CONFIGBUILDER_HPP

#include "config/Config.hpp"
#include "config/ParsedConfig.hpp"
#include "config/ParsedServer.hpp"
#include <cstddef>
#include <string>
#include <vector>

class ConfigBuilder
{
public:
  static Config build(const ParsedConfig& parsed);

private:
  struct DirectiveHandler
  {
    const char* key;
    void (*func)(const std::vector<std::string>& values, Config& config);
  };

  static void buildDirectives(const ParsedConfig::Directive& directives,
                              Config& config);
  static void buildServers(const std::vector<ParsedServer>& servers,
                           Config& config);

  static void checkDirectiveHandler(const std::string& key,
                                    const std::vector<std::string>& values,
                                    Config& config);

  // add all the setters
  static void setRoot(const std::vector<std::string>& value, Config& config);
  static void setMaxBodySize(const std::vector<std::string>& value,
                             Config& config);
  static void setTimeout(const std::vector<std::string>& value, Config& config);
  // static void setErrorPages(const std::vector<std::string>& value,
  //                           Config& config);

  static const DirectiveHandler _handlers[];
  static const std::size_t _handlersSize;
};

#endif
