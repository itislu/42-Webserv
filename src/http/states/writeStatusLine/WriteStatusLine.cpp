#include "WriteStatusLine.hpp"

#include <client/Client.hpp>
#include <http/http.hpp>
#include <http/states/writeHeaderLines/WriteHeaderLines.hpp>
#include <utils/Buffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

/* ************************************************************************** */
// INIT

Logger& WriteStatusLine::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

WriteStatusLine::WriteStatusLine(Client* context)
  : IState(context)
  , _client(context)
{
  _log.info() << "WriteStatusLine\n";
}

/**
 * status-line = HTTP-version SP status-code SP [ reason-phrase ]
 */
void WriteStatusLine::run()
{
  Buffer& buff = _client->getOutBuff();
  buff.add("HTTP/1.1 ");
  buff.add(_client->getResponse().getStatusCode().toString());
  buff.add(http::CRLF);
  _client->getStateHandler().setState<WriteHeaderLines>();
}

/* ************************************************************************** */
// PRIVATE
