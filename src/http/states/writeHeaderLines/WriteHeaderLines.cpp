#include "WriteHeaderLines.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/http.hpp>
#include <http/states/writeBody/WriteBody.hpp>
#include <string>
#include <utils/IBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <ctime>

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
  headers.addHeader("Date", _makeHttpDate());
  headers.addHeader("Server", "webserv"); // TODO from config probaly ?
  headers.addHeader("Connection", "close");

  IBuffer& outBuffer = _client->getOutBuff();
  outBuffer.append(headers.toString());
  outBuffer.append(http::CRLF);

  _client->getStateHandler().setState<WriteBody>();
}

/* ************************************************************************** */
// PRIVATE

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
std::string WriteHeaderLines::_makeHttpDate()
{
  const std::time_t now = std::time(0);

  // Convert to GMT/UTC
  std::tm gmt = {};
  gmt = *std::gmtime(&now);

  const int bufSize = 64;
  char buf[bufSize];
  // Format: "Sat, 15 Nov 2025 18:43:29 GMT"
  (void)std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &gmt);
  return std::string(buf);
}
// NOLINTEND(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
