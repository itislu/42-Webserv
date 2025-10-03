#include "ParseUri.hpp"
#include "http/states/readStartLineState/IReadStartLineState.hpp"
#include "http/states/readStartLineState/ReadStartLine.hpp"
#include <cstddef>

/* ************************************************************************** */
// PUBLIC

ParseUri::ParseUri(ReadStartLine* state)
  : IReadStartLineState(state)
  , _client(state->getClient())
{
}

ParseUri::~ParseUri() {}



/* ************************************************************************** */
// PRIVATE

ParseUri::ParseUri()
  : IReadStartLineState(NULL)
  , _client(NULL)
{
}

ParseUri::ParseUri(const ParseUri& other)
  : IReadStartLineState(other.getState())
  , _client(other._client)
{
  *this = other;
}

ParseUri& ParseUri::operator=(const ParseUri& other)
{
  if (this != &other) {
    // todo copy logic
  }
  return *this;
}
