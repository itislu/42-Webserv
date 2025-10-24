#include "WriteStatusLine.hpp"

#include <client/Client.hpp>
#include <utils/state/IState.hpp>

/* ************************************************************************** */
// PUBLIC

WriteStatusLine::WriteStatusLine(Client* context)
  : IState(context)
  , _client(context)
{
}

void WriteStatusLine::run()
{
  _client->getStateHandler().setDone();
}

/* ************************************************************************** */
// PRIVATE
