#include "ReadBody.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/Response.hpp>
#include <http/StatusCode.hpp>
#include <http/abnfRules/generalRules.hpp>
#include <http/abnfRules/http11Rules.hpp>
#include <http/abnfRules/ruleIds.hpp>
#include <http/headerUtils.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/utils/HeaderParser.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/string.hpp>
#include <utils/abnfRules/Extractor.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/Rule.hpp>
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
#include <vector>

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

  _chunkInfoRule().setResultMap(&_results);
  _endOfLineRule().setResultMap(&_results);
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
  static const ft::shared_ptr<SequenceRule> rule = chunkInfoRule();
  return *rule;
}

LiteralRule& ReadBody::_endOfLineRule()
{
  static const ft::shared_ptr<LiteralRule> rule = endOfLineRule();
  return *rule;
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
  const bool hasTransferEncoding = headers.contains(header::transferEncoding);
  const bool hasContentLength = headers.contains("Content-Length");
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
  const std::string strLen = headers.at("Content-Length");
  if (!_setBodyLength(strLen, std::ios::dec)) {
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
  std::vector<std::string> elements =
    convertHeaderList(headers.at(header::transferEncoding));
  if (elements.empty()) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "ReadBody: Transfer-Encoding invalid\n";
    return;
  }

  const std::string finalEncoding = ft::to_lower(elements.back());
  if (finalEncoding != "chunked") {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "ReadBody: chunked is not final encoding\n";
    return;
  }

  std::size_t chunkedCount = 0;
  for (std::size_t i = 0; i < elements.size(); ++i) {
    std::string& encoding = elements[i];
    if (ft::to_lower(encoding) == "chunked") {
      ++chunkedCount;
    } else {
      _client->getResponse().setStatusCode(StatusCode::NotImplemented);
      _log.error() << "ReadBody: encoding not implemented\n";
      return;
    }
    if (chunkedCount > 1) {
      _client->getResponse().setStatusCode(StatusCode::BadRequest);
      _log.error() << "ReadBody: multiple chunked encoding\n";
      return;
    }
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
  Response& response = _client->getResponse();
  IInOutBuffer& buffer = _client->getInBuff();
  Headers& trailers = _client->getRequest().getTrailers();
  const HeaderParser::Result result =
    _headerParser.parseIntoStruct(buffer, trailers);

  switch (result) {
    case HeaderParser::EndOfBuffer:
      // header is not complete
      break;
    case HeaderParser::EndOfHeaders:
      _done = true;
      break;
    case HeaderParser::SyntaxError:
      _log.error() << "ReadBody: trailer syntax error\n";
      response.setStatusCode(StatusCode::BadRequest);
      break;
    case HeaderParser::FieldLineTooLarge:
      response.setStatusCode(StatusCode::RequestHeaderFieldsTooLarge);
      _log.error() << "ReadBody: trailer line too large\n";
      break;
    case HeaderParser::HeaderTooLarge:
      response.setStatusCode(StatusCode::RequestHeaderFieldsTooLarge);
      _log.error() << "ReadBody: trailer too large\n";
      break;
    case HeaderParser::InvalidHeader:
      // will only be set if custom validator used
      if (response.getStatusCode() == StatusCode::Ok) {
        _log.error() << "ReadBody: validator failed to set error status\n";
        response.setStatusCode(StatusCode::BadRequest);
      }
      break;
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

void ReadBody::_setChunkSize(const std::string& hexNum)
{
  _setBodyLength(hexNum, std::ios::hex);
}

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

bool ReadBody::_setBodyLength(const std::string& numStr, std::ios::fmtflags fmt)
{
  std::istringstream iss(numStr);
  iss.flags(fmt);
  iss >> _bodyLength;
  if (iss.fail()) {
    // Overflow is the only possible error since grammar ensures valid digits.
    _client->getResponse().setStatusCode(StatusCode::ContentTooLarge);
    _log.error() << "ReadBody: body length too large\n";
    return false;
  }
  // todo get from config ?
  const std::size_t maxBodySize = 2147483647;
  if (_bodyLength > maxBodySize) {
    _client->getResponse().setStatusCode(StatusCode::ContentTooLarge);
    _log.error() << "ReadBody: body length bigger than max body size\n";
    return false;
  }
  _log.info() << "body length: " << _bodyLength << '\n';
  return true;
}
