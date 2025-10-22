#pragma once
#ifndef PARSE_VERSION_HPP
#define PARSE_VERSION_HPP

#include "utils/abnfRules/SequenceRule.hpp"
#include "utils/BufferReader.hpp"
#include <utils/state/IState.hpp>

class Client;
class ReadStartLine;

/* ************************************************************************** */
class ParseVersion : public IState<ReadStartLine>
{
public:
  explicit ParseVersion(ReadStartLine* context);
  ~ParseVersion();

  void run();

private:
  ParseVersion();
  ParseVersion(const ParseVersion& other);
  ParseVersion& operator=(const ParseVersion& other);

  void _extractVersion();

  Client* _client;
  BufferReader _buffReader;
  SequenceRule _sequenze;
  bool _init;
};

#endif // PARSE_URI_HPP
