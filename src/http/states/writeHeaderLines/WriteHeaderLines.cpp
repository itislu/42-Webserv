#include "WriteHeaderLines.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/http.hpp>
#include <http/states/writeBody/WriteBody.hpp>
#include <string>
#include <utils/Buffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/logger/LoggerHandler.hpp>
#include <utils/state/IState.hpp>

/* ************************************************************************** */
// PUBLIC

WriteHeaderLines::WriteHeaderLines(Client* context)
  : IState<Client>(context)
  , _client(context)
  , _log(&LoggerHandler::getInstance(log::http))
{
  _log->info("WriteHeaderLines");
}

void WriteHeaderLines::run()
{
  Headers& headers = _client->getResponse().getHeaders();
  headers.addHeader("Date", "Wed, 29 Oct 2025 12:00:00 GMT"); // TODO
  headers.addHeader("Server", "webserv"); // TODO from config probaly
  headers.addHeader("Content-Type", "text/html");
  headers.addHeader("Connection", "close");

  Buffer& outBuffer = _client->getOutBuff();
  outBuffer.add(headers.toString());
  outBuffer.add(http::CRLF);

  _client->getStateHandler().setState<WriteBody>();
}

/* ************************************************************************** */
// PRIVATE
