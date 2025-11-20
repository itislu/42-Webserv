#include "WriteStatusLine.hpp"

#include <client/Client.hpp>
#include <http/http.hpp>
#include <http/states/writeHeaderLines/WriteHeaderLines.hpp>
#include <utils/IBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <string>

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
  IBuffer& buff = _client->getOutBuff();

  std::string statusLine;
  statusLine.append("HTTP/1.1 ");
  statusLine.append(_client->getResponse().getStatusCode().toString());
  statusLine.append(http::CRLF);

  const IBuffer::ExpectVoid res = buff.append(statusLine);
  if (!res.has_value()) {
    // todo
    // what should we do now?
  }
  _client->getStateHandler().setState<WriteHeaderLines>();
}

/* ************************************************************************** */
// PRIVATE
