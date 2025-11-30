#include "ReadBody.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/StatusCode.hpp>
#include <http/abnfRules/generalRules.hpp>
#include <http/abnfRules/headerLineRules.hpp>
#include <http/abnfRules/http11Rules.hpp>
#include <http/abnfRules/ruleIds.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <libftpp/algorithm.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/string.hpp>
#include <libftpp/utility.hpp>
#include <utils/abnfRules/Extractor.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/RuleResult.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/buffer/IBuffer.hpp>
#include <utils/buffer/IInOutBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

/* ************************************************************************** */
// INIT

Logger& ReadBody::_log = Logger::getInstance(LOG_HTTP);
const std::size_t ReadBody::_readChunkSize;

/* ************************************************************************** */
// PUBLIC

ReadBody::ReadBody(Client* context)
  : IState<Client>(context)
  , _client(context)
  , _buffReader()
  , _bodyLength(0)
  , _consumed(0)
  , _chunkedState(ReadChunkInfo)
  , _fixedLengthBody(false)
  , _chunkedBody(false)
  , _done(false)
{
  _log.info() << *_client << " ReadBody\n";

  _determineBodyFraming();
  _buffReader.init(&_client->getInBuff());
  _chunkInfoRule().setBufferReader(&_buffReader);
  _endOfLineRule().setBufferReader(&_buffReader);
  _fieldLineRule().setBufferReader(&_buffReader);

  _chunkInfoRule().setResultMap(&_results);
  _endOfLineRule().setResultMap(&_results);
  _fieldLineRule().setResultMap(&_results);
}

void ReadBody::run()
try {
  if (_fixedLengthBody) {
    _readFixedLengthBody();
  } else if (_chunkedBody) {
    _readChunkedBody();
  }

  if (_done || _client->getResponse().getStatusCode() != StatusCode::Ok) {
    getContext()->getStateHandler().setState<PrepareResponse>();
  }
} catch (const std::exception& e) {
  _log.error() << *_client << " ReadBody: " << e.what() << "\n";
  getContext()->getResponse().setStatusCode(StatusCode::InternalServerError);
  getContext()->getStateHandler().setState<PrepareResponse>();
}

/* ************************************************************************** */
// PRIVATE

/**
 * chunk          = chunk-size [ chunk-ext ] CRLF
 *                  chunk-data CRLF
 *
 * ! only first part
 */
SequenceRule& ReadBody::_chunkInfoRule()
{
  static SequenceRule rule;
  static bool init = false;
  if (!init) {
    init = true;
    rule.addRule(chunkInfoRule());
  }
  return rule;
}

SequenceRule& ReadBody::_endOfLineRule()
{
  static SequenceRule rule;
  static bool init = false;
  if (!init) {
    init = true;
    rule.addRule(endOfLineRule());
  }
  return rule;
}

/**
 * field-line CRLF
 */
SequenceRule& ReadBody::_fieldLineRule()
{
  static SequenceRule rule;
  static bool init = false;
  if (!init) {
    init = true;
    rule.addRule(fieldLinePartRule());
  }
  return rule;
}

Extractor<ReadBody>& ReadBody::_chunkExtractor()
{
  static Extractor<ReadBody> extractor;
  static bool init = false;
  if (!init) {
    init = true;
    extractor.addMapItem(ChunkSize, &ReadBody::_setChunkSize);
    extractor.addMapItem(ChunkExt, &ReadBody::_setChunkExt);
  }
  return extractor;
}

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
  if (strBodyLen.fail() || !strBodyLen.eof()) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "ReadBody: invalid Content-Length\n";
    return;
  }
  if (_bodyLength == 0) {
    _done = true;
    return;
  }
  _fixedLengthBody = true;
}

void ReadBody::_validateTransferEncoding()
{
  const Headers& headers = _client->getRequest().getHeaders();
  const std::string& value = headers.at("Transfer-Encoding");
  const std::string valueLower = ft::to_lower(value);
  if (!ft::contains_subrange(valueLower, std::string("chunked"))) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "ReadBody: transfer encoding unsupported\n";
    return;
  }
  _chunkedBody = true;
}

void ReadBody::_readFixedLengthBody()
{
  _readBody();
  if (_consumed >= _bodyLength) {
    _done = true;
  }
}

void ReadBody::_readChunkedBody()
{
  while (_readingOk()) {
    if (_chunkedState == ReadChunkInfo) {
      _readChunkInfo();
    }
    if (_chunkedState == ReadChunkData) {
      _readChunkData();
    }
    if (_chunkedState == ReadChunkDataEnd) {
      _readChunkDataEnd();
    }
    if (_chunkedState == ReadTrailerSection) {
      _readTrailerSection();
    }
  }
}

void ReadBody::_readChunkInfo()
{
  _results.clear();
  _buffReader.resetPosInBuff();
  _chunkInfoRule().reset();
  if (!_chunkInfoRule().matches()) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "ReadBody: failed to parse chunk size\n";
    return;
  }

  if (_chunkInfoRule().reachedEnd()) {
    _chunkExtractor().run(*this, _results, _client->getInBuff());
    _client->getInBuff().removeFront(_buffReader.getPosInBuff());

    // last-chunk ?
    if (_bodyLength == 0) {
      _chunkedState = ReadTrailerSection;
      return;
    }
    _chunkedState = ReadChunkData;
    _consumed = 0;
  }
}

void ReadBody::_readChunkData()
{
  _readBody();
  if (_consumed >= _bodyLength) {
    _chunkedState = ReadChunkDataEnd;
  }
}

void ReadBody::_readChunkDataEnd()
{
  _buffReader.resetPosInBuff();
  _endOfLineRule().reset();
  if (!_endOfLineRule().matches()) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "ReadBody: failed to parse end of line of chunk body\n";
  }

  if (_endOfLineRule().reachedEnd()) {
    _chunkedState = ReadChunkInfo;
    _client->getInBuff().removeFront(_buffReader.getPosInBuff());
  }
}

void ReadBody::_readTrailerSection()
{
  _buffReader.resetPosInBuff();
  while (_readingOk()) {
    _results.clear();
    _fieldLineRule().reset();
    _endOfLineRule().reset();

    if (_fieldLineRule().matches()) {
      if (_fieldLineRule().reachedEnd()) {
        const std::string fieldLine = _extractPart(FieldLinePart);
        _addLineToHeaders(fieldLine);
      }
    } else if (_hasEndOfLine()) {
      if (_endOfLineRule().reachedEnd()) {
        _extractPart(EndOfLine);
        _done = true;
        return;
      }
    } else {
      _client->getResponse().setStatusCode(StatusCode::BadRequest);
      _log.error() << "ReadBody: failed to read trailer section\n";
      _done = true;
      return;
    }
  }
}

bool ReadBody::_readingOk()
{
  const StatusCode& statuscode = _client->getResponse().getStatusCode();
  if (statuscode != StatusCode::Ok) {
    return false;
  }
  return !_done && !_buffReader.reachedEnd();
}

bool ReadBody::_hasEndOfLine()
{
  _buffReader.resetPosInBuff();
  return _endOfLineRule().matches();
}

std::string ReadBody::_extractPart(const Rule::RuleId& ruleId)
{
  const RuleResult& result = _results[ruleId];
  const std::size_t index = result.getEnd();
  const std::string res = _client->getInBuff().consumeFront(index);
  _buffReader.resetPosInBuff();
  return res;
}

void ReadBody::_addLineToHeaders(const std::string& line)
{
  const std::size_t index = line.find(':');
  if (index == std::string::npos) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "ReadBody: invalid fieldline\n";
    return;
  }
  const std::string name = line.substr(0, index);
  const std::string value = line.substr(index + 1, line.size());
  Headers& headers = _client->getRequest().getHeaders();
  headers.addHeader(name, value);
}

// NOLINTBEGIN(misc-const-correctness)
void ReadBody::_setChunkSize(const std::string& hexNum)
{
  char* end = FT_NULLPTR;
  const short base = 16;
  const long value = std::strtol(hexNum.c_str(), &end, base);
  if (end == hexNum.c_str() || value < 0) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "ReadBody: failed to parse chunk size\n";
    return;
  }
  _log.info() << "body size: " << value << '\n';
  _bodyLength = static_cast<std::size_t>(value);
}
// NOLINTEND(misc-const-correctness)

void ReadBody::_setChunkExt(const std::string& value)
{
  _chunkExtension = value;
}

/**
 * @brief Used for fixed size and chunked reading
 *
 * - read until requested size is consumed
 */
void ReadBody::_readBody()
{
  IInOutBuffer& inBuffer = _client->getInBuff();
  Request& request = _client->getRequest();

  std::size_t toConsume = std::min(_readChunkSize, _bodyLength - _consumed);
  toConsume = std::min(toConsume, inBuffer.size());
  if (toConsume == 0) {
    return;
  }

  if (_contentTooLarge(toConsume)) {
    return;
  }

  const IBuffer::RawBytes rawBytes = inBuffer.consumeRawFront(toConsume);
  request.getBody().append(rawBytes, toConsume);
  _consumed += toConsume;
}

/**
 * @brief check if max body size is exceeded
 */
bool ReadBody::_contentTooLarge(std::size_t newBytes)
{
  // todo get from config ?
  const std::size_t maxBodySize = 2147483647;
  const std::size_t sizeBody = _client->getRequest().getBody().size();
  if (sizeBody + newBytes > maxBodySize) {
    _client->getResponse().setStatusCode(StatusCode::ContentTooLarge);
    _log.error() << "ReadBody: content too large\n";
    return true;
  }
  return false;
}
