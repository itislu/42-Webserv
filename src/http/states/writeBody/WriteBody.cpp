#include "WriteBody.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Response.hpp>
#include <http/StatusCode.hpp>
#include <http/http.hpp>
#include <http/states/prepareResponse/HandleError.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/buffer/IBuffer.hpp>
#include <utils/buffer/IInBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <algorithm>
#include <cstddef>
#include <exception>
#include <ios>
#include <sstream>

/* ************************************************************************** */
// INIT

Logger& WriteBody::_log = Logger::getInstance(LOG_HTTP);
const std::size_t WriteBody::_outBufferLimit;
const std::size_t WriteBody::_chunkSize;

/* ************************************************************************** */
// PUBLIC

WriteBody::WriteBody(Client* context)
  : IState<Client>(context)
  , _client(context)
  , _done(false)
  , _chunked(false)
  , _fixedLength(false)
  , _smartBuffer(FT_NULLPTR)
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
    _smartBuffer = &_client->getOutBuffQueue().getSmartBuffer();
  }
}

void WriteBody::_handleFixedLengthBody()
{
  _client->getOutBuffQueue().append(_client->getResponse().getBody());
  _done = true;
}

/**
 * Write more into the outBuffer than the EventManager will send to keep poll
 * enabled. Also limit the amount written into SmartBuffer so it stays a
 * MemoryBuffer.
 *
 * chunk          = chunk-size [ chunk-ext ] CRLF
 *                  chunk-data CRLF
 */
void WriteBody::_handleChunkedBody()
{
  const ft::shared_ptr<IInBuffer>& body = _client->getResponse().getBody();
  while (!_done && _smartBuffer->size() < _outBufferLimit) {
    if (body->isEmpty()) {
      _smartBuffer->append("0\r\n\r\n");
      _done = true;
      break;
    }

    const std::size_t currChunkSize = std::min(body->size(), _chunkSize);
    const IBuffer::RawBytes rawBytes = body->consumeRawFront(currChunkSize);

    std::ostringstream oss;
    oss << std::hex << std::nouppercase << rawBytes.size() << http::CRLF;
    _smartBuffer->append(oss.str());
    _smartBuffer->append(rawBytes, rawBytes.size());
    _smartBuffer->append(http::CRLF);

    if (body->isEmpty()) {
      _done = true;
      // if we ever send trailers the second CRLF should be removed
      _smartBuffer->append("0\r\n\r\n");
    }
  }
}
