#ifndef DIRECTIVEHANDLER_HPP
#define DIRECTIVEHANDLER_HPP

#include "libftpp/array.hpp"
#include "config/ParsedConfig.hpp"
#include <string>
#include <vector>

template<typename ConfigType>
class DirectiveHandler
{
public:
  struct Entry
  {
    const char* key;
    void (*func)(const std::vector<std::string>& values, ConfigType& config);
  };

  static void checkDirectiveHandler(const std::string& key,
                                    const std::vector<std::string>& values,
                                    ConfigType& config);

  static void buildDirectives(const ParsedConfig::Directive& directives,
                              ConfigType& config);

private:
  // ======= Setters =======
  static void setRoot(const std::vector<std::string>& values,
                      ConfigType& config);
  static void setIndex(const std::vector<std::string>& values,
                       ConfigType& config);
  static void setTimeout(const std::vector<std::string>& values,
                         ConfigType& config);
  static void setMaxBodySize(const std::vector<std::string>& values,
                             ConfigType& config);
  static void setErrorPage(const std::vector<std::string>& values,
                           ConfigType& config);
  static void setPorts(const std::vector<std::string>& values,
                       ConfigType& config);
  static void setHostnames(const std::vector<std::string>& values,
                           ConfigType& config);
  static void setAllowedMethods(const std::vector<std::string>& values,
                                ConfigType& config);
  static void setAutoIndex(const std::vector<std::string>& values,
                           ConfigType& config);
  static void setCgi(const std::vector<std::string>& values,
                     ConfigType& config);
  static void setCgiPass(const std::vector<std::string>& values,
                         ConfigType& config);
  static void setCgiExtension(const std::vector<std::string>& values,
                              ConfigType& config);
  static void setRedirect(const std::vector<std::string>& values,
                          ConfigType& config);
  static void setRedirection(const std::vector<std::string>& values,
                             ConfigType& config);
  static void setRedirectCode(const std::vector<std::string>& values,
                              ConfigType& config);

  template <typename T>
  struct EntryWrapper {
    static T entries[];
  };

  static Entry(&_getEntries())[5];
};

#include "DirectiveHandler.tpp"

#endif
