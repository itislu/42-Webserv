#pragma once
#ifndef PARSE_URI_HPP
#define PARSE_URI_HPP

#include <http/Uri.hpp>
#include <libftpp/memory.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/state/IState.hpp>

#include <string>

class Client;
class ReadRequestLine;

/* ************************************************************************** */
class ParseUri : public IState<ReadRequestLine>
{
public:
  explicit ParseUri(ReadRequestLine* context);

  void run();

private:
  enum ParseUriState
  {
    ParseScheme,
    ParseAuthority,
    ParsePath,
    ParseQuery,
    ParseFragment,
    ParseDone
  };

  void _parseScheme();
  void _parseAuthority();
  void _parsePath();
  void _parseQuery();
  void _parseFragment();

  void _updateState(ParseUriState nextState);
  void _initSequence();
  std::string _extractPart(const Rule::RuleId& ruleId);
  std::string _extractPartWithoutCurrChar(const Rule::RuleId& ruleId);
  bool _partFound(const Rule::RuleId& ruleId);

  Client* _client;
  ParseUriState _parseState;
  BufferReader _buffReader;
  Uri _tmpUri;

  ft::unique_ptr<SequenceRule> _sequence;
  Rule::ResultMap _results;
  bool _initParser;
};

#endif
