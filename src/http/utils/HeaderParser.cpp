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
  _validator = ft::move(validator);
}

HeaderParser::Result HeaderParser::parseIntoStruct(IInOutBuffer& buffer,
                                                   Headers& headers)
{
  _initValidator();
  _reader.init(&buffer);
  _endOfLineRule().setBufferReader(&_reader);
  _fieldLineRule().setBufferReader(&_reader);
  const std::size_t startPos = 0;
  while (!_reader.reachedEnd()) {
    if (_isValidHeaderLine(startPos)) {
      if (_fieldLineRule().reachedEnd()) {
        const ft::optional<Result> optRes = _addHeader(buffer, headers);
        if (optRes.has_value()) {
          return *optRes;
        }
      }
    } else if (_isValidEndOfLine(startPos)) {
      if (_endOfLineRule().reachedEnd()) {
        return _removeEndOfLine(buffer);
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
  _reader.setPosInBuff(0);
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
  _reader.setPosInBuff(0);
  _fieldLineRule().reset();
  const bool matches = _fieldLineRule().matches();
  _bytesRead = _reader.getPosInBuff() - startPos;
  return matches;
}

ft::optional<HeaderParser::Result> HeaderParser::_addHeader(
  IInOutBuffer& buffer,
  Headers& headers)
{
  if (_validator->fieldLineTooLarge(_bytesRead)) {
    return FieldLineTooLarge;
  }
  if (_validator->headerTooLarge(_bytesRead)) {
    return HeaderTooLarge;
  }
  std::string name;
  std::string value;
  _extractHeaderParts(buffer, name, value);
  if (!_validator->isValid(name, value)) {
    return InvalidHeader;
  }
  headers.addHeader(name, value);
  return ft::nullopt;
}

void HeaderParser::_extractHeaderParts(IInOutBuffer& buffer,
                                       std::string& name,
                                       std::string& value)
{
  const std::string line = buffer.consumeFront(_bytesRead);
  const std::size_t index = line.find(':');
  name = line.substr(0, index);
  value = line.substr(index + 1, line.size());
  _reader.resetPosInBuff();
}

HeaderParser::Result HeaderParser ::_removeEndOfLine(IInOutBuffer& buffer)
{
  if (_validator->headerTooLarge(_bytesRead)) {
    return HeaderTooLarge;
  }
  buffer.removeFront(_bytesRead);
  return EndOfHeaders;
}
