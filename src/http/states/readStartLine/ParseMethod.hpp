#pragma once
#ifndef PARSE_METHOD_HPP
#define PARSE_METHOD_HPP

#include "utils/abnfRules/SequenceRule.hpp"
#include <utils/state/IState.hpp>
#include "utils/BufferReader.hpp"

class Client;
class ReadStartLine;

/* ************************************************************************** */
class ParseMethod : public IState<ReadStartLine>
{
public:
  explicit ParseMethod(ReadStartLine* context);
  ~ParseMethod();

  void run();

private:
  ParseMethod();
  ParseMethod(const ParseMethod& other);
  ParseMethod& operator=(const ParseMethod& other);

  void _extractMethod();
  static bool _isValid(char chr);

  Client* _client;
  BufferReader _buffReader;
  SequenceRule _sequenze;
  bool _init;
};

#endif // PARSE_METHOD_HPP
