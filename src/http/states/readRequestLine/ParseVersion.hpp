#pragma once
#ifndef PARSE_VERSION_HPP
#define PARSE_VERSION_HPP

#include <utils/BufferReader.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

class Client;
class ReadRequestLine;

/* ************************************************************************** */
class ParseVersion : public IState<ReadRequestLine>
{
public:
  explicit ParseVersion(ReadRequestLine* context);

  void run();

private:
  void _init();
  void _extractVersion();
  void _validateVersion();

  Client* _client;
  BufferReader _buffReader;
  SequenceRule _sequence;
  static Logger& _log;
};

#endif
