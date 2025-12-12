#include "WriteHeaderLines.hpp"
#include "http/headerUtils.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/StatusCode.hpp>
#include <http/http.hpp>
#include <http/states/prepareResponse/HandleError.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/writeBody/WriteBody.hpp>
#include <libftpp/string.hpp>
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
  , _buffer(&_client->getOutBuffQueue().getSmartBuffer())
{
  _log.info() << "WriteHeaderLines\n";
}

void WriteHeaderLines::run()
try {
  Headers& headers = _client->getResponse().getHeaders();
  headers.setHeader("Date", _makeHttpDate());
  headers.setHeader("Server", "webserv"); // TODO from config probably ?

  _setConnectionHeader();

  _buffer->append(headers.toString());
  _buffer->append(http::CRLF);

  _client->getStateHandler().setState<WriteBody>();
} catch (const std::exception& e) {
  _log.error() << *_client << " WriteHeaderLines: " << e.what() << "\n";
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

void WriteHeaderLines::_setConnectionHeader()
{
  Request& request = _client->getRequest();
  const Response& response = _client->getResponse();
  const Headers& reqHeaders = request.getHeaders();
  Headers& headers = _client->getResponse().getHeaders();

  if (_client->closeConnection() || !response.getStatusCode().is2xxCode()) {
    headers.setHeader(header::connection, "close");
    return;
  }

  std::string conn;
  if (reqHeaders.contains(header::connection)) {
    conn = ft::to_lower(reqHeaders.at(header::connection));
  }

  // close connection present
  if (conn == "close") {
    headers.setHeader(header::connection, "close");
    return;
  }

  // HTTP/1.1 -> connection persist
  if (request.getVersion() == "HTTP/1.1") {
    headers.setHeader(header::connection, "keep-alive");
    return;
  }

  // HTTP/1.0 + keep-alive -> connection persist
  if (request.getVersion() == "HTTP/1.0" && conn == "keep-alive") {
    headers.setHeader(header::connection, "keep-alive");
    return;
  }

  headers.setHeader(header::connection, "close");
}
