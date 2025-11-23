#include "ReadBody.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/StatusCode.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <libftpp/algorithm.hpp>
#include <libftpp/string.hpp>
#include <utils/IBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <algorithm>
#include <cstddef>
#include <sstream>
#include <string>

/* ************************************************************************** */
// INIT

Logger& ReadBody::_log = Logger::getInstance(LOG_HTTP);
const std::size_t ReadBody::_chunkSize;

/* ************************************************************************** */
// PUBLIC

ReadBody::ReadBody(Client* context)
  : IState<Client>(context)
  , _client(context)
  , _initialized(false)
  , _fixedLengthBody(false)
  , _chunked(false)
  , _done(false)
  , _bodyLength(0)
  , _consumed(0)
{
  _log.info() << "ReadBody\n";
}

void ReadBody::run()
try {
  if (!_initialized) {
    _initialized = true;
    _determineBodyFraming();
  }

  if (_fixedLengthBody && _bodyLength > 0) {
    _readFixedLengthBody();
  } else if (_chunked) {
    _readChunkedBody();
  }

  if (_done) {
    getContext()->getStateHandler().setState<PrepareResponse>();
  }
} catch (const IBuffer::BufferException& e) {
  _log.error() << "ReadBody: " << e.what() << '\n';
  getContext()->getResponse().setStatusCode(StatusCode::InternalServerError);
  getContext()->getStateHandler().setState<PrepareResponse>();
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
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "ReadBody: has Transfer-Encoding AND Content-Length\n";
    return;
  }
  if (hasContentLength) {
    _validateContentLength();
    return;
  }
  if (hasTransferEncoding) {
    _validateTransferEncoding();
    return;
  }

  // No Body
  _log.info() << "ReadBody: no body framing defined\n";
  _done = true;
}

void ReadBody::_validateContentLength()
{
  const Headers& headers = _client->getRequest().getHeaders();
  std::istringstream strBodyLen(headers.at("Content-Length"));
  strBodyLen >> _bodyLength;
  if (strBodyLen.fail()) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "ReadBody: invalid Content-Length\n";
    return;
  }
  if (_bodyLength == 0) {
    _done = true;
  }
  _fixedLengthBody = true;
}

void ReadBody::_validateTransferEncoding()
{
  const Headers& headers = _client->getRequest().getHeaders();
  const std::string& value = headers.at("Transfer-Encoding");
  if (!ft::contains_subrange(ft::to_lower(value), "chunked")) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "ReadBody: transfer encoding unsupported\n";
    return;
  }
  _chunked = true;
}

void ReadBody::_readFixedLengthBody()
{
  IBuffer& inBuffer = _client->getInBuff();
  Request& request = _client->getRequest();

  std::size_t toConsume = std::min(_chunkSize, _bodyLength - _consumed);
  toConsume = std::min(toConsume, inBuffer.size());
  if (toConsume == 0) {
    return;
  }

  IBuffer::ExpectStr res = inBuffer.consumeFront(toConsume);
  request.getBody().append(*res);
  _consumed += toConsume;

  if (_consumed >= _bodyLength) {
    _done = true;
  }
}

void ReadBody::_readChunkedBody()
{
  // todo
}
