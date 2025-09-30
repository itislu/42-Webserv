#include "IState.hpp"
#include "client/Client.hpp"

/* ************************************************************************** */
// PUBLIC

IState::IState() {}

IState::IState(Client* client)
  : client(client)
{
}

IState::~IState() {}

IState::IState(const IState& other)
  : client(other.client)
{
  *this = other;
}

IState& IState::operator=(const IState& other)
{
  if (this != &other) {
    // todo copy logic
  }
  return *this;
}

/* ************************************************************************** */
// PRIVATE
