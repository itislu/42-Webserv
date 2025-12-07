#include "WriteBody.hpp"
#include "utils/buffer/SmartBuffer.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/StatusCode.hpp>
#include <http/http.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/buffer/IBuffer.hpp>
#include <utils/buffer/IInBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <exception>
#include <ios>
#include <sstream>
#include <string>

/* ************************************************************************** */
// INIT

Logger& WriteBody::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

WriteBody::WriteBody(Client* context)
  : IState<Client>(context)
  , _client(context)
  , _outBuffer(FT_NULLPTR)
  , _done(false)
  , _chunked(false)
  , _fixedLength(false)
{
  _log.info() << "WriteBody\n";
  _defineBodyFraming();
}

void WriteBody::run()
try {
  if (_fixedLength) {
    _handleFixedLengthBody();
  } else if (_chunked) {
    _handleChunkedBody();
  }

  if (_done) {
    _log.info() << *_client << " WriteBody: " << "done\n";
    getContext()->getStateHandler().setDone();
  }
} catch (const std::exception& e) {
  _log.error() << *_client << " WriteBody: " << e.what() << '\n';
  getContext()->getResponse().setStatusCode(StatusCode::InternalServerError);
  throw;
}

/* ************************************************************************** */
// PRIVATE

void WriteBody::_defineBodyFraming()
{
  if (_client->getResponse().getBody() == FT_NULLPTR) {
    _done = true;
    return;
  }

  const Headers& responseHeaders = _client->getResponse().getHeaders();
  if (responseHeaders.contains("Content-Length")) {
    _fixedLength = true;
  } else {
    _chunked = true;
    _outBuffer = &_client->getOutBuffQueue().getSmartBuffer();
  }
}

void WriteBody::_handleFixedLengthBody()
{
  _client->getOutBuffQueue().append(_client->getResponse().getBody());
  _done = true;
}

/**
 * Write more into the outBuffer than the Client will send to keep poll enabled.
 *
 * chunk          = chunk-size [ chunk-ext ] CRLF
 *                  chunk-data CRLF
 */
void WriteBody::_handleChunkedBody()
{
  // Ensure amount written into the SmartBuffer keeps it a MemoryBuffer.
  assert(SmartBuffer::getMemoryToFileThreshold() > Client::maxChunk * 2);
  if (_outBuffer->size() > Client::maxChunk) {
    return;
  }

  const ft::shared_ptr<IInBuffer>& body = _client->getResponse().getBody();

  if (body->isEmpty()) {
    _handleLastChunk();
    return;
  }

  const std::size_t currChunkSize = std::min(body->size(), Client::maxChunk);
  const IBuffer::RawBytes rawBytes = body->consumeRawFront(currChunkSize);

  std::ostringstream oss;
  oss << std::hex << std::nouppercase << rawBytes.size() << http::CRLF;
  _outBuffer->append(oss.str());
  _outBuffer->append(rawBytes, rawBytes.size());
  _outBuffer->append(http::CRLF);

  if (body->isEmpty()) {
    _handleLastChunk();
  }
}

/**
 * last-chunk     = 1*("0") [ chunk-ext ] CRLF
 */
void WriteBody::_handleLastChunk()
{
  // if we ever send trailers the second CRLF should be removed
  static const std::string lastChunk =
    std::string("0").append(http::CRLF).append(http::CRLF);

  _outBuffer->append(lastChunk);
  _done = true;
}
