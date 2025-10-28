#pragma once
#ifndef PARSE_METHOD_HPP
#define PARSE_METHOD_HPP

#include <utils/BufferReader.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/state/IState.hpp>

class Client;
class ReadStartLine;

/* ************************************************************************** */
class ParseMethod : public IState<ReadStartLine>
{
public:
  explicit ParseMethod(ReadStartLine* context);

  void run();

private:
  void _init();
  void _extractMethod();

  Client* _client;
  BufferReader _buffReader;
  SequenceRule _sequence;
};

#endif
