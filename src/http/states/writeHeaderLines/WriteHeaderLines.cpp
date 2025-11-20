#include "WriteHeaderLines.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/http.hpp>
#include <http/states/writeBody/WriteBody.hpp>
#include <string>
#include <utils/IBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

/* ************************************************************************** */
// INIT

Logger& WriteHeaderLines::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

WriteHeaderLines::WriteHeaderLines(Client* context)
  : IState<Client>(context)
  , _client(context)
{
  _log.info() << "WriteHeaderLines\n";
}

void WriteHeaderLines::run()
{
  Headers& headers = _client->getResponse().getHeaders();
  headers.addHeader("Date", "Wed, 29 Oct 2025 12:00:00 GMT"); // TODO
  headers.addHeader("Server", "webserv"); // TODO from config probaly
  headers.addHeader("Connection", "close");

  IBuffer& outBuffer = _client->getOutBuff();
  outBuffer.append(headers.toString());
  outBuffer.append(http::CRLF);

  _client->getStateHandler().setState<WriteBody>();
}

/* ************************************************************************** */
// PRIVATE
