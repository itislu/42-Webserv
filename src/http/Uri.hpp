#pragma once
#ifndef URI_HPP
#define URI_HPP

#include <string>

/* ************************************************************************** */
class Uri
{
public:
  const std::string& getScheme() const;
  void setScheme(const std::string& str);
  const std::string& getAuthority() const;
  void setAuthority(const std::string& str);
  const std::string& getPath() const;
  void setPath(const std::string& str);
  const std::string& getQuery() const;
  void setQuery(const std::string& str);
  const std::string& getFragment() const;
  void setFragment(const std::string& str);

  std::string toString();

private:
  std::string _scheme;
  std::string _authority;
  std::string _path;
  std::string _query;
  std::string _fragment;
};

#endif
