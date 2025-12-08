#include "HeaderParser.hpp"

#include <http/Headers.hpp>
#include <http/abnfRules/generalRules.hpp>
#include <http/abnfRules/headerLineRules.hpp>
#include <http/utils/BaseHeaderValidator.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/optional.hpp>
#include <libftpp/utility.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/buffer/IInOutBuffer.hpp>

#include <cstddef>
#include <string>

/* ************************************************************************** */
// PUBLIC

void HeaderParser::setValidator(ft::shared_ptr<BaseHeaderValidator> validator)
{
  _customValidatorAvailable = validator != FT_NULLPTR;
  _validator = ft::move(validator);
}

HeaderParser::Result HeaderParser::parseIntoStruct(IInOutBuffer& buffer,
                                                   Headers& headers)
{
  _initValidator();
  _reader.init(&buffer);
  _endOfLineRule().setBufferReader(&_reader);
  _fieldLineRule().setBufferReader(&_reader);
  _startPos = 0;
  while (!_reader.reachedEnd()) {
    if (_isValidHeaderLine(_startPos)) {
      if (_fieldLineRule().reachedEnd()) {
        const ft::optional<Result> optRes = _runValidator(buffer);
        if (optRes.has_value()) {
          return *optRes;
        }
        _addHeader(buffer, headers);
      }
    } else if (_isValidEndOfLine(_startPos)) {
      if (_endOfLineRule().reachedEnd()) {
        const ft::optional<Result> optRes = _validateEndOfLine();
        if (optRes.has_value()) {
          return *optRes;
        }
        buffer.removeFront(_bytesRead);
        return EndOfHeaders;
      }
    } else {
      return SyntaxError;
    }
  }
  return EndOfBuffer;
}

HeaderParser::Result HeaderParser::validateHeaderPart(IInBuffer& buffer)
{
  _initValidator();
  _reader.init(&buffer);
  _endOfLineRule().setBufferReader(&_reader);
  _fieldLineRule().setBufferReader(&_reader);
  while (!_reader.reachedEnd()) {
    _startPos = _reader.getPosInBuff();
    if (_isValidHeaderLine(_startPos)) {
      if (_fieldLineRule().reachedEnd()) {
        const ft::optional<Result> optRes = _runValidator(buffer);
        if (optRes.has_value()) {
          return *optRes;
        }
      }
    } else if (_isValidEndOfLine(_startPos)) {
      if (_endOfLineRule().reachedEnd()) {
        const ft::optional<Result> optRes = _validateEndOfLine();
        if (optRes.has_value()) {
          return *optRes;
        }
        return EndOfHeaders;
      }
    } else {
      return SyntaxError;
    }
  }
  return EndOfBuffer;
}

/* ************************************************************************** */
// PRIVATE

/**
 * CRLF
 */
LiteralRule& HeaderParser::_endOfLineRule()
{
  static const ft::shared_ptr<LiteralRule> sequence = endOfLineRule();
  return *sequence;
}

/**
 * field-line (with CRLF) = field-line CRLF
 * field-line             = field-name ":" OWS field-value OWS
 */
SequenceRule& HeaderParser::_fieldLineRule()
{
  static const ft::shared_ptr<SequenceRule> sequence = fieldLinePartRule();
  return *sequence;
}

void HeaderParser::_initValidator()
{
  if (_validator != FT_NULLPTR) {
    return;
  }
  _validator = ft::make_shared<BaseHeaderValidator>();
}

/**
 * @brief true when line is CRLF
 */
bool HeaderParser::_isValidEndOfLine(std::size_t startPos)
{
  _endOfLineRule().reset();
  const bool matches = _endOfLineRule().matches();
  _bytesRead = _reader.getPosInBuff() - startPos;
  return matches;
}

/**
 * @brief true when line matches 'field-line CRLF'
 */
bool HeaderParser::_isValidHeaderLine(std::size_t startPos)
{
  _fieldLineRule().reset();
  const bool matches = _fieldLineRule().matches();
  _bytesRead = _reader.getPosInBuff() - startPos;
  return matches;
}

ft::optional<HeaderParser::Result> HeaderParser::_runValidator(
  IInBuffer& buffer)
{
  if (_validator->fieldLineTooLarge(_bytesRead)) {
    return FieldLineTooLarge;
  }
  if (_validator->headerTooLarge(_bytesRead)) {
    return HeaderTooLarge;
  }
  if (_customValidatorAvailable) {
    _getHeaderParts(buffer);
    _headerPartsSet = true;
    if (!_validator->isValid(_name, _value)) {
      return InvalidHeader;
    }
  } else {
    _headerPartsSet = false;
  }
  return ft::nullopt;
}

void HeaderParser::_addHeader(IInOutBuffer& buffer, Headers& headers)
{
  if (!_headerPartsSet) {
    _getHeaderParts(buffer);
  }
  buffer.removeFront(_bytesRead);
  _reader.resetPosInBuff();
  headers.addHeader(_name, _value);
}

void HeaderParser::_getHeaderParts(IInBuffer& buffer)
{
  const std::string line = buffer.getStr(_startPos, _bytesRead);
  const std::size_t index = line.find(':');
  _name = line.substr(0, index);
  _value = line.substr(index + 1, line.size());
}

ft::optional<HeaderParser::Result> HeaderParser ::_validateEndOfLine()
{
  if (_validator->headerTooLarge(_bytesRead)) {
    return HeaderTooLarge;
  }
  return ft::nullopt;
}
