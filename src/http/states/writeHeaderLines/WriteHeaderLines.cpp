#include "WriteHeaderLines.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/StatusCode.hpp>
#include <http/http.hpp>
#include <http/states/prepareResponse/HandleError.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/writeBody/WriteBody.hpp>
#include <libftpp/utility.hpp>
#include <stdexcept>
#include <utils/IBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <ctime>
#include <string>

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
try {
  Headers& headers = _client->getResponse().getHeaders();
  headers.addHeader("Date", _makeHttpDate());
  headers.addHeader("Server", "webserv"); // TODO from config probably ?
  headers.addHeader("Connection", "close");

  IBuffer& outBuffer = _client->getOutBuff();
  outBuffer.append(headers.toString());
  outBuffer.append(http::CRLF);

  _client->getStateHandler().setState<WriteBody>();
} catch (const std::runtime_error& e) {
  _log.error() << "WriteHeaderLines: " << e.what() << "\n";
  _client->getResponse().setStatusCode(StatusCode::InternalServerError);
  throw;
}

/* ************************************************************************** */
// PRIVATE

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
std::string WriteHeaderLines::_makeHttpDate()
{
  const std::time_t now = std::time(FT_NULLPTR);

  // Convert to GMT/UTC
  const std::tm* const gmt = std::gmtime(&now);
  if (gmt == FT_NULLPTR) {
    throw std::runtime_error("WriteHeaderLines: failed to make http date\n");
  }

  const int bufSize = 64;
  char buf[bufSize];
  // Format: "Sat, 15 Nov 2025 18:43:29 GMT"
  if (std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmt) == 0) {
    throw std::runtime_error("WriteHeaderLines: failed to make http date\n");
  }
  return std::string(buf);
}
// NOLINTEND(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
