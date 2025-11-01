#pragma once
#ifndef PARSE_METHOD_HPP
#define PARSE_METHOD_HPP

#include <utils/BufferReader.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

class Client;
class ReadRequestLine;

/* ************************************************************************** */
class ParseMethod : public IState<ReadRequestLine>
{
public:
  explicit ParseMethod(ReadRequestLine* context);

  void run();

private:
  void _init();
  void _extractMethod();

  Client* _client;
  BufferReader _buffReader;
  SequenceRule _sequence;
  Logger* _log;
};

#endif
