#include "ReadBody.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/StatusCode.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <libftpp/algorithm.hpp>
#include <utils/Buffer.hpp>
#include <utils/logger/LoggerHandler.hpp>
#include <utils/state/IState.hpp>

#include <sstream>
#include <string>

/* ************************************************************************** */
// PUBLIC

ReadBody::ReadBody(Client* context)
  : IState<Client>(context)
  , _client(context)
  , _log(&LoggerHandler::getInstance(log::http))
  , _initialized(false)
  , _fixedLengthBody(false)
  , _bodyLength(0)
{
  _log->info("ReadBody");
}

void ReadBody::run()
{
  if (!_initialized) {
    _initialized = true;
    _determineBodyFraming();
  }
  if (_fixedLengthBody) {
    _readFixedLengthBody();
  } else {
    _readChunkedBody();
  }
}

/* ************************************************************************** */
// PRIVATE

// If “Transfer-Encoding” exists
// If it contains “chunked” → _fixedLengthBody = false; state = ReadBodyChunked.
// Else → unsupported (501 or 400).
// Else if “Content-Length” exists
// Parse integer; if it’s valid (≥ 0) → _fixedLengthBody = true; _bodyLength =
// N. If missing or invalid → 400 Bad Request. Else No body → skip to
// PrepareResponse.

void ReadBody::_determineBodyFraming()
{
  Headers& headers = _client->getRequest().getHeaders();
  const bool hasTransferEncoding = headers.contains("Transfer-Encoding");
  const bool hasContentLength = headers.contains("Content-Length");
  if (hasContentLength && hasTransferEncoding) {
    _log->error("ReadBody: has Transfer-Encoding AND Content-Length");
  } else if (hasContentLength) {
    _fixedLengthBody = true;
    std::stringstream strBodyLen(headers["Content-Length"]);
    strBodyLen >> _bodyLength;
    if (!strBodyLen.fail()) {
      return;
    }
    _log->error("ReadBody: failed to read content length");
  } else if (hasTransferEncoding) {
    const std::string& value = headers["Transfer-Encoding"];
    if (ft::contains_subrange(value, "chunked")) {
      return;
    }
    _log->error("ReadBody: transfer encoding unsupported");
  } else {
    _log->error("ReadBody: no body framing defined");
  }

  // Error case
  _client->getResponse().setStatusCode(StatusCode::BadRequest);
  _client->getStateHandler().setState<PrepareResponse>();
}

void ReadBody::_readFixedLengthBody()
{
  // todo for large bodies, write to file
  // or make buffer smart so the buffer writes to a file
  // when it gets to big
  Buffer& inBuffer = _client->getInBuff();
  Request& request = _client->getRequest();
  const long size = static_cast<long>(inBuffer.getSize());
  request.getBody().add(inBuffer.consume(size));
}

void ReadBody::_readChunkedBody()
{
  // todo
}
