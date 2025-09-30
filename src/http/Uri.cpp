#include "Uri.hpp"

/* ************************************************************************** */
// PUBLIC

Uri::Uri() {}

Uri::~Uri() {}

Uri::Uri(const Uri& other) { *this = other; }

Uri& Uri::operator=(const Uri& other) {
  if (this != &other) {
    // todo copy logic
  }
  return *this;
}

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
