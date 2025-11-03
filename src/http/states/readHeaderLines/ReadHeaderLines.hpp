#pragma once
#ifndef READ_HEADER_LINES_HPP
#define READ_HEADER_LINES_HPP

#include <libftpp/memory.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/state/IState.hpp>

#include <string>

class Client;

/* ************************************************************************** */
class ReadHeaderLines : public IState<Client>
{
public:
  explicit ReadHeaderLines(Client* context);

  void run();

private:
  void _init();
  void _readLines();
  bool _hasEndOfLine();
  std::string _extractPart(const Rule::RuleId& ruleId);
  void _addLineToHeaders(const std::string& line);

  Client* _client;
  ft::unique_ptr<SequenceRule> _fieldLine;
  // Could be unique_ptr, but gets assigned from a function returning
  // shared_ptr.
  ft::shared_ptr<LiteralRule> _endOfLine;
  BufferReader _buffReader;
  Rule::ResultMap _results;
};

#endif
