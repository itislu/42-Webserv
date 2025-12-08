#pragma once
#ifndef HEADER_PARSER_HPP
#define HEADER_PARSER_HPP

#include <http/utils/BaseHeaderValidator.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/optional.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

#include <cstddef>
#include <string>

class IInOutBuffer;
class IInBuffer;
class Headers;

/* ************************************************************************** */
class HeaderParser
{
public:
  enum Result
  {
    EndOfBuffer,
    EndOfHeaders,
    SyntaxError,
    FieldLineTooLarge,
    HeaderTooLarge,
    InvalidHeader
  };

  void setValidator(ft::shared_ptr<BaseHeaderValidator> validator);

  Result parseIntoStruct(IInOutBuffer& buffer, Headers& headers);
  // void validateHeaders(IInBuffer& buffer);

private:
  static LiteralRule& _endOfLineRule();
  static SequenceRule& _fieldLineRule();
  void _initValidator();
  bool _isValidEndOfLine(std::size_t startPos);
  bool _isValidHeaderLine(std::size_t startPos);
  ft::optional<Result> _addHeader(IInOutBuffer& buffer, Headers& headers);
  void _extractHeaderParts(IInOutBuffer& buffer,
                           std::string& name,
                           std::string& value);
  Result _removeEndOfLine(IInOutBuffer& buffer);

  ft::shared_ptr<BaseHeaderValidator> _validator;
  BufferReader _reader;
  std::size_t _bytesRead;
};

#endif
