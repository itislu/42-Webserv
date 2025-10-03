#include "IReadStartLineState.hpp"
#include <cstddef>

/* ************************************************************************** */
// PUBLIC

IReadStartLineState::IReadStartLineState(ReadStartLine* state)
  : _state(state)
{
}

IReadStartLineState::~IReadStartLineState() {}

ReadStartLine* IReadStartLineState::getState() const
{
  return _state;
}

/* ************************************************************************** */
// PRIVATE

IReadStartLineState::IReadStartLineState()
  : _state(NULL)
{
}

IReadStartLineState::IReadStartLineState(const IReadStartLineState& other)
  : _state(other._state)
{
  *this = other;
}

IReadStartLineState& IReadStartLineState::operator=(
  const IReadStartLineState& other)
{
  if (this != &other) {
    // todo copy logic
  }
  return *this;
}
