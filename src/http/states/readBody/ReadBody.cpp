#include "ReadBody.hpp"

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
// PUBLIC

ReadBody::ReadBody(Client* context)
  : IState<Client>(context)
  , _client(context)
  , _log(&Logger::getInstance(logFiles::http))
  , _initialized(false)
  , _fixedLengthBody(false)
  , _bodyLength(0)
{
  _log->info() << "ReadBody\n";
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

void ReadBody::_determineBodyFraming()
{
  Headers& headers = _client->getRequest().getHeaders();
  const bool hasTransferEncoding = headers.contains("Transfer-Encoding");
  const bool hasContentLength = headers.contains("Content-Length");
  // todo add grama rules for these headers
  if (hasContentLength && hasTransferEncoding) {
    _log->error() << "ReadBody: has Transfer-Encoding AND Content-Length\n";
  } else if (hasContentLength) {
    _fixedLengthBody = true;
    std::stringstream strBodyLen(headers["Content-Length"]);
    strBodyLen >> _bodyLength;
    if (!strBodyLen.fail()) {
      return;
    }
    _log->error() << "ReadBody: failed to read content length\n";
  } else if (hasTransferEncoding) {
    const std::string& value = headers["Transfer-Encoding"];
    if (ft::contains_subrange(value, "chunked")) {
      return;
    }
    _log->error() << "ReadBody: transfer encoding unsupported\n";
  } else {
    _log->info() << "ReadBody: no body framing defined\n";
    return;
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
