#pragma once
#ifndef URI_HPP
#define URI_HPP

#include <string>

/* ************************************************************************** */
class Uri
{
public:
  Uri();
  ~Uri();
  Uri(const Uri& other);
  Uri& operator=(const Uri& other);

  std::string getRaw() const;
  void setRaw(std::string& rawUri);

private:
  std::string _raw;
  // querry
  // host
};

#endif // URI_HPP
