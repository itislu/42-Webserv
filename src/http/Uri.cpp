#include "Uri.hpp"
#include <string>

/* ************************************************************************** */
// PUBLIC

std::string Uri::getRaw() const
{
  return _raw;
}

void Uri::setRaw(std::string& rawUri)
{
  _raw = rawUri;
}

/* ************************************************************************** */
// PRIVATE
