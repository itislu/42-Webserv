#include "WriteStatusLine.hpp"

#include <client/Client.hpp>
#include <http/StatusCode.hpp>
#include <http/http.hpp>
#include <http/states/writeHeaderLines/WriteHeaderLines.hpp>
#include <utils/buffer/SmartBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <exception>
#include <string>

/* ************************************************************************** */
// INIT

Logger& WriteStatusLine::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

WriteStatusLine::WriteStatusLine(Client* context)
  : IState(context)
  , _client(context)
  , _buffer(_client->getOutBuffQueue().getSmartBuffer())
{
  _log.info() << "WriteStatusLine\n";
}

/**
 * status-line = HTTP-version SP status-code SP [ reason-phrase ]
 */
void WriteStatusLine::run()
try {
  std::string statusLine;
  statusLine.append("HTTP/1.1 ");
  statusLine.append(_client->getResponse().getStatusCode().toString());
  statusLine.append(http::CRLF);

  _buffer->append(statusLine);
  _client->getStateHandler().setState<WriteHeaderLines>();
} catch (const std::exception& e) {
  _log.error() << *_client << " WriteStatusLine: " << e.what() << "\n";
  _client->getResponse().setStatusCode(StatusCode::InternalServerError);
  throw;
}

/* ************************************************************************** */
// PRIVATE
