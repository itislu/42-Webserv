#include "WriteHeaderLines.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/StatusCode.hpp>
#include <http/http.hpp>
#include <http/states/prepareResponse/HandleError.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/writeBody/WriteBody.hpp>
#include <libftpp/utility.hpp>
#include <utils/buffer/SmartBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <ctime>
#include <exception>
#include <stdexcept>
#include <string>

/* ************************************************************************** */
// INIT

Logger& WriteHeaderLines::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

WriteHeaderLines::WriteHeaderLines(Client* context)
  : IState<Client>(context)
  , _client(context)
  , _buffer(_client->getOutBuffQueue().getSmartBuffer())
{
  _log.info() << "WriteHeaderLines\n";
}

void WriteHeaderLines::run()
try {
  Headers& headers = _client->getResponse().getHeaders();
  headers.addHeader("Date", _makeHttpDate());
  headers.addHeader("Server", "webserv"); // TODO from config probably ?
  headers.addHeader("Connection", "close");

  _buffer->append(headers.toString());
  _buffer->append(http::CRLF);

  _client->getStateHandler().setState<WriteBody>();
} catch (const std::exception& e) {
  _log.error() << *_client << " WriteHeaderLines: " << e.what() << "\n";
  _client->getResponse().setStatusCode(StatusCode::InternalServerError);
  throw;
}

/* ************************************************************************** */
// PRIVAT

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
std::string WriteHeaderLines::_makeHttpDate()
{
  const std::time_t now = std::time(0);

  // Convert to GMT/UTC
  std::tm gmt = {};
  if (gmtime_r(&now, &gmt) == FT_NULLPTR) {
    throw std::runtime_error("WriteHeaderLines: failed to make http date\n");
  }

  const int bufSize = 64;
  char buf[bufSize];
  // Format: "Sat, 15 Nov 2025 18:43:29 GMT"
  if (std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &gmt) == 0) {
    throw std::runtime_error("WriteHeaderLines: failed to make http date\n");
  }
  return std::string(buf);
}
// NOLINTEND(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
