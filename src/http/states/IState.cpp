#include "IState.hpp"
#include "client/Client.hpp"

/* ************************************************************************** */
// PUBLIC

IState::IState(Client* client)
  : _client(client)
{
}

IState::~IState() {}

Client* IState::getClient() const
{
  return _client;
}

/* ************************************************************************** */
// PRIVATE

IState::IState()
  : _client(NULL)
{
}

IState::IState(const IState& other)
  : _client(other._client)
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
