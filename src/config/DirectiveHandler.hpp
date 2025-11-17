#ifndef DIRECTIVEHANDLER_HPP
#define DIRECTIVEHANDLER_HPP

#include "config/Config.hpp"
#include "config/ConfigTypes.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include <string>
#include <vector>

namespace config {


/*
  CONFIG  : root, maxbodysize, errorpage, timeout,
  SERVER  : root, maxbodysize, errorpage, timeout, index, allowed, port, hosts
  LOCATION: root, maxbodysize, errorpage, xxxxxxx, index, allowed, ...
*/

template<typename ConfigType>
class DirectiveHandlerBase
{
public:
  struct Entry
  {
    const char* key;
    void (*func)(const std::vector<std::string>& values, ConfigType& config);
  };

  // ======= Setters =======
  static void setRoot(const std::vector<std::string>& values,
                      ConfigType& config);
  static void setMaxBodySize(const std::vector<std::string>& values,
                             ConfigType& config);
  static void setErrorPage(const std::vector<std::string>& values,
                           ConfigType& config);
};

template<typename ConfigType>
class Entries;

template<>
class Entries<Config> : public DirectiveHandlerBase<Config>
{
public:
  static const Entry entries[];

private:
  static void setTimeout(const std::vector<std::string>& values,
                         Config& config);
};

template<>
class Entries<ServerConfig> : public DirectiveHandlerBase<ServerConfig>
{
public:
  static const Entry entries[];

private:
  static void setTimeout(const std::vector<std::string>& values,
                         ServerConfig& config);
  static void setIndex(const std::vector<std::string>& values,
                       ServerConfig& config);
  static void setAllowedMethods(const std::vector<std::string>& values,
                                ServerConfig& config);
  static void setPorts(const std::vector<std::string>& values,
                       ServerConfig& config);
  static void setHostnames(const std::vector<std::string>& values,
                           ServerConfig& config);
};

template<>
class Entries<LocationConfig> : public DirectiveHandlerBase<LocationConfig>
{
public:
  static const Entry entries[];

private:
  static void setIndex(const std::vector<std::string>& values,
                       LocationConfig& config);
  static void setAllowedMethods(const std::vector<std::string>& values,
                                LocationConfig& config);
  static void setAutoIndex(const std::vector<std::string>& values,
                           LocationConfig& config);
  static void setCgi(const std::vector<std::string>& values,
                     LocationConfig& config);
  static void setCgiPass(const std::vector<std::string>& values,
                         LocationConfig& config);
  static void setCgiExtension(const std::vector<std::string>& values,
                              LocationConfig& config);
  static void setRedirect(const std::vector<std::string>& values,
                          LocationConfig& config);
  static void setRedirection(const std::vector<std::string>& values,
                             LocationConfig& config);
  static void setRedirectCode(const std::vector<std::string>& values,
                              LocationConfig& config);
};

template<typename ConfigType>
class DirectiveHandler : private Entries<ConfigType>
{
public:
  static void checkDirectiveHandler(const std::string& key,
                                    const std::vector<std::string>& values,
                                    ConfigType& config);

  static void buildDirectives(const DirectiveMap& directives,
                              ConfigType& config);
};

} // namespace config


#include "DirectiveHandler.tpp" // IWYU pragma: export

#endif
