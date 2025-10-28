#include "WriteStatusLine.hpp"

#include <client/Client.hpp>
#include <utils/Buffer.hpp>
#include <utils/state/IState.hpp>

/* ************************************************************************** */
// PUBLIC

WriteStatusLine::WriteStatusLine(Client* context)
  : IState(context)
  , _client(context)
{
}

/**
 * status-line = HTTP-version SP status-code SP [ reason-phrase ]
 */
void WriteStatusLine::run()
{
  Buffer& buff = _client->getOutBuff();
  buff.add("HTTP/1.1 ");
  buff.add(_client->getResponse().getStatusCode().toString());
  _client->getStateHandler().setDone();
}

/* ************************************************************************** */
// PRIVATE
