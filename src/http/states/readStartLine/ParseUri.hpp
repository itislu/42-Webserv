#pragma once
#ifndef PARSE_URI_HPP
#define PARSE_URI_HPP

#include <http/Uri.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/state/IState.hpp>

#include <string>

class Client;
class ReadStartLine;

/* ************************************************************************** */
class ParseUri : public IState<ReadStartLine>
{
public:
  explicit ParseUri(ReadStartLine* context);
  ~ParseUri();

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

  ParseUri();
  ParseUri(const ParseUri& other);
  ParseUri& operator=(const ParseUri& other);

  void _parseScheme();
  void _parseAuthority();
  void _parsePath();
  void _parseQuery();
  void _parseFragment();

  void _updateState(ParseUriState nextState);
  std::string _extractPartIfAvailable(Rule::RuleId ruleId);
  std::string _extractPart();
  std::string _extractPartWithoutCurrChar();

  Client* _client;
  ParseUriState _parseState;
  BufferReader _buffReader;
  Uri _tmpUri;

  SequenceRule* _sequence;
  Rule::ResultMap _results;
  bool _initParser;
};

#endif // PARSE_URI_HPP
