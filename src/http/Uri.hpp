#pragma once
#ifndef URI_HPP
#define URI_HPP

#include <http/Authority.hpp>

#include <string>

/* ************************************************************************** */
class Uri
{
public:
  const std::string& getScheme() const;
  const Authority& getAuthority() const;
  Authority& getAuthority();
  const std::string& getPath() const;
  const std::string& getQuery() const;
  const std::string& getFragment() const;

  void setScheme(const std::string& str);
  void setPath(const std::string& str);
  void setQuery(const std::string& str);
  void setFragment(const std::string& str);

  std::string toString() const;

private:
  std::string _scheme;
  Authority _authority;
  std::string _path;
  std::string _query;
  std::string _fragment;
};

#endif
