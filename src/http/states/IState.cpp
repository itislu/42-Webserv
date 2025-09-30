#include "IState.hpp"

/* ************************************************************************** */
// PUBLIC

IState::IState() {}

IState::~IState() {}

IState::IState(const IState& other) { *this = other; }

IState& IState::operator=(const IState& other) {
  if (this != &other) {
    // todo copy logic
  }
  return *this;
}

/* ************************************************************************** */
// PRIVATE
