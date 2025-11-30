#pragma once
#ifndef READ_BODY_HPP
#define READ_BODY_HPP

#include <utils/abnfRules/Extractor.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstddef>
#include <string>

class Client;

/* ************************************************************************** */
class ReadBody : public IState<Client>
{
public:
  explicit ReadBody(Client* context);

  void run();

private:
  enum ChunkedState
  {
    ReadChunkInfo,
    ReadChunkData,
    ReadChunkDataEnd,
    ReadTrailerSection
  };

  static SequenceRule& _chunkInfoRule();
  static SequenceRule& _endOfLineRule();
  static SequenceRule& _fieldLineRule();
  static Extractor<ReadBody>& _chunkExtractor();

  void _determineBodyFraming();
  void _validateContentLength();
  void _validateTransferEncoding();

  void _readFixedLengthBody();

  void _readChunkedBody();
  void _readChunkInfo();
  void _readChunkData();
  void _readChunkDataEnd();
  void _readTrailerSection();
  bool _readingOk();
  bool _hasEndOfLine();
  std::string _extractPart(const Rule::RuleId& ruleId);
  void _addLineToHeaders(const std::string& line);
  void _setChunkSize(const std::string& hexNum);
  void _setChunkExt(const std::string& value);

  void _readBody();
  bool _contentTooLarge(std::size_t newBytes);

  static Logger& _log;
  static const std::size_t _readChunkSize = 1024;

  Client* _client;
  Rule::ResultMap _results;

  std::size_t _bodyLength;
  std::size_t _consumed;
  ChunkedState _chunkedState;
  std::string _chunkExtension;

  bool _fixedLengthBody;
  bool _chunkedBody;
  bool _done;
};

#endif
