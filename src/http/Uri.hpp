#pragma once
#ifndef URI_HPP
#define URI_HPP

#include <string>

/* ************************************************************************** */
class Uri
{
public:
  std::string getRaw() const;
  void setRaw(std::string& rawUri);

private:
  std::string _raw;
  std::string _scheme;
  std::string _authority;
  std::string _path;
  std::string _query;
  std::string _fragment;
};

#endif // URI_HPP
