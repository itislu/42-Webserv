#include "ReadHeaderLines.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/StatusCode.hpp>
#include <http/abnfRules/generalRules.hpp>
#include <http/abnfRules/headerLineRules.hpp>
#include <http/abnfRules/ruleIds.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/readBody/ReadBody.hpp>
#include <http/states/validateRequest/ValidateRequest.hpp>
#include <http/states/writeStatusLine/WriteStatusLine.hpp>
#include <libftpp/memory.hpp>
#include <utils/BufferReader.hpp>
#include <utils/IBuffer.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/RuleResult.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstddef>
#include <string>

/* **************************************************************************
 */
// INIT

Logger& ReadHeaderLines::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ReadHeaderLines::ReadHeaderLines(Client* context)
  : IState(context)
  , _client(context)
  , _buffReader()
  , _sizeHeaders(0)
  , _done(false)
{
  _log.info() << "ReadHeaderLines\n";
  _init();
}

/* ************************************************************************** */
// PRIVATE

/**
 * @brief ### Read header lines of a HTTP request
 *
 * HTTP-message   = start-line CRLF
 *                  *( field-line CRLF )
 *                  CRLF
 *                  [ message-body ]
 */
void ReadHeaderLines::run()
try {
  _readLines();
  if (_done || _client->getResponse().getStatusCode() != StatusCode::Ok) {
    _setNextState();
    return;
  }
} catch (const IBuffer::BufferException& e) {
  _log.error() << "ReadHeaderLines: " << e.what() << "\n";
  getContext()->getResponse().setStatusCode(StatusCode::InternalServerError);
  getContext()->getStateHandler().setState<PrepareResponse>();
}

void ReadHeaderLines::_init()
{
  _buffReader.init(&_client->getInBuff());

  _fieldLine = fieldLinePartRule();
  _fieldLine->setBufferReader(&_buffReader);
  _fieldLine->setResultMap(&_results);

  _endOfLine = endOfLineRule();
  _endOfLine->setBufferReader(&_buffReader);
  _endOfLine->setResultMap(&_results);
}

bool ReadHeaderLines::_readingOk()
{
  const StatusCode& statuscode = _client->getResponse().getStatusCode();
  if (statuscode != StatusCode::Ok) {
    return false;
  }
  if (_buffReader.reachedEnd()) {
    return false;
  }
  return true;
}

void ReadHeaderLines::_readLines()
{
  _buffReader.resetPosInBuff();
  while (_readingOk()) {
    _results.clear();
    _fieldLine->reset();
    _endOfLine->reset();

    if (_fieldLine->matches()) {
      if (_fieldLine->reachedEnd()) {
        const std::string fieldLine = _extractPart(FieldLinePart);
        _addLineToHeaders(fieldLine);
      }
    } else if (_hasEndOfLine()) {
      if (_endOfLine->reachedEnd()) {
        _extractPart(EndOfLine);
        _done = true;
        return;
      }
    } else {
      _log.error() << "ReadHeaderLines: Bad Request: Headers:\n";
      _log.error() << _client->getRequest().getHeaders().toString() << '\n';
      _client->getResponse().setStatusCode(StatusCode::BadRequest);
      _done = true;
      return;
    }
  }
}

void ReadHeaderLines::_setNextState()
{
  const StatusCode& statusCode = _client->getResponse().getStatusCode();

  if (statusCode == StatusCode::Ok) {
    getContext()->getStateHandler().setState<ValidateRequest>();
  } else {
    getContext()->getStateHandler().setState<PrepareResponse>();
  }
}

bool ReadHeaderLines::_hasEndOfLine()
{
  _buffReader.resetPosInBuff();
  return _endOfLine->matches();
}

std::string ReadHeaderLines::_extractPart(const Rule::RuleId& ruleId)
{
  const RuleResult& result = _results[ruleId];
  const std::size_t index = result.getEnd();

  if (_headerTooLarge(index)) {
    return "";
  }
  const std::string part = _client->getInBuff().consumeFront(index);
  _buffReader.resetPosInBuff();
  return part;
}

void ReadHeaderLines::_addLineToHeaders(const std::string& line)
{
  const std::size_t index = line.find(':');
  const std::string name = line.substr(0, index);
  const std::string value = line.substr(index + 1, line.size());
  Headers& headers = _client->getRequest().getHeaders();
  headers.addHeader(name, value);
}

bool ReadHeaderLines::_headerTooLarge(std::size_t newBytes)
{
  // todo values from config ??

  const std::size_t maxFieldLineSize = 4242;
  if (newBytes > maxFieldLineSize) {
    _client->getResponse().setStatusCode(
      StatusCode::RequestHeaderFieldsTooLarge);
    _log.error() << "ReadHeaderLines: field line too large\n";
    return true;
  }

  const std::size_t maxHeaderSize = 8042;
  if (_sizeHeaders + newBytes > maxHeaderSize) {
    _client->getResponse().setStatusCode(
      StatusCode::RequestHeaderFieldsTooLarge);
    _log.error() << "ReadHeaderLines: header too large\n";
    return true;
  }

  _sizeHeaders += newBytes;
  return false;
}
