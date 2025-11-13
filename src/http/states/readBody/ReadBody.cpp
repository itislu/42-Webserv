#include "ReadBody.hpp"
#include "libftpp/string.hpp"

#include <algorithm>
#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/StatusCode.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <libftpp/algorithm.hpp>
#include <utils/Buffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <sstream>
#include <string>

/* ************************************************************************** */
// INIT

Logger& ReadBody::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ReadBody::ReadBody(Client* context)
  : IState<Client>(context)
  , _client(context)
  , _bodyLength(0)
  , _consumed(0)
  , _initialized(false)
  , _fixedLengthBody(false)
  , _chunked(false)
  , _done(false)
{
  _log.info() << "ReadBody\n";
}

void ReadBody::run()
{
  if (!_initialized) {
    _initialized = true;
    _determineBodyFraming();
  }

  if (_fixedLengthBody) {
    _readFixedLengthBody();
  } else if (_chunked) {
    _readChunkedBody();
  }

  if (_done) {
    getContext()->getStateHandler().setState<PrepareResponse>();
  }
}

/* ************************************************************************** */
// PRIVATE

void ReadBody::_determineBodyFraming()
{
  const Headers& headers = _client->getRequest().getHeaders();
  const bool hasTransferEncoding = headers.contains("Transfer-Encoding");
  const bool hasContentLength = headers.contains("Content-Length");
  // todo add grammar rules for these headers
  if (hasContentLength && hasTransferEncoding) {
    _log.error() << "ReadBody: has Transfer-Encoding AND Content-Length\n";
  } else if (hasContentLength) {
    if (_isValidContentLength()) {
      return;
    }
    _log.error() << "ReadBody: invalid Content-Length\n";
  } else if (hasTransferEncoding) {
    if (_isValidTransferEncoding()) {
      return;
    }
    _log.error() << "ReadBody: transfer encoding unsupported\n";
  } else {
    _log.info() << "ReadBody: no body framing defined\n";
    _done = true;
    return; // OK no body
  }

  // Error case
  _client->getResponse().setStatusCode(StatusCode::BadRequest);
  _client->getStateHandler().setState<PrepareResponse>();
}

bool ReadBody::_isValidContentLength()
{
  const Headers& headers = _client->getRequest().getHeaders();
  std::istringstream strBodyLen(headers.at("Content-Length"));
  strBodyLen >> _bodyLength;
  if (strBodyLen.fail()) {
    return false;
  }
  _fixedLengthBody = true;
  return true;
}

bool ReadBody::_isValidTransferEncoding()
{
  const Headers& headers = _client->getRequest().getHeaders();
  const std::string& value = headers.at("Transfer-Encoding");
  if (!ft::contains_subrange(ft::to_lower(value), "chunked")) {
    return false;
  }
  _chunked = true;
  return true;
}

void ReadBody::_readFixedLengthBody()
{
  // TODO: For large bodies, write to file or use a smart buffer that flushes to
  // file.
  Buffer& inBuffer = _client->getInBuff();
  Request& request = _client->getRequest();

  const long size = static_cast<long>(inBuffer.getSize());
  if (size <= 0 || _done) {
    return;
  }

  const long chunkSize = 1024;
  long toConsume = std::min<long>(chunkSize, _bodyLength - _consumed);
  toConsume = std::min<long>(toConsume, size);

  if (toConsume <= 0) {
    return;
  }

  request.getBody().add(inBuffer.consume(toConsume));
  _consumed += toConsume;

  if (_consumed >= _bodyLength) {
    _done = true;
  }
}

void ReadBody::_readChunkedBody()
{
  // todo
}
