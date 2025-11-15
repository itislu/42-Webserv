#pragma once
#ifndef PARSE_METHOD_HPP
#define PARSE_METHOD_HPP

#include <libftpp/memory.hpp>
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
  static void _initSequence();
  void _initState();
  void _extractMethod();

  static Logger& _log;
  static ft::shared_ptr<SequenceRule> _sequence;

  Client* _client;
  BufferReader _buffReader;
};

#endif
