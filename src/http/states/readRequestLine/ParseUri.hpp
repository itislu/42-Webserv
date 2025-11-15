#pragma once
#ifndef PARSE_URI_HPP
#define PARSE_URI_HPP

#include <http/Authority.hpp>
#include <http/Uri.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/Extractor.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

class Client;
class ReadRequestLine;

/* ************************************************************************** */
class ParseUri : public IState<ReadRequestLine>
{
public:
  explicit ParseUri(ReadRequestLine* context);

  void run();

  static void setMaxUriLength(long length);
  static const long defaultMaxUriLength = 8042;

private:
  static SequenceRule& _sequence();
  static bool _isAbsoluteForm(const Rule::ResultMap& results);
  static Extractor<Uri>& _uriExtractorOriginForm();
  static Extractor<Uri>& _uriExtractorAbsoluteForm();
  static Extractor<Authority>& _authExtractor();
  void _extractParts(const Rule::ResultMap& results);
  bool _requestTargetTooLong();

  static Logger& _log;
  static long _maxUriLength;

  Client* _client;
  BufferReader _buffReader;
  Uri _tmpUri;

  Rule::ResultMap _results;
};

#endif
