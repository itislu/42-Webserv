#pragma once
#ifndef END_RULE_HPP
#define END_RULE_HPP

#include "Rule.hpp"

#include <libftpp/memory.hpp>
#include <utils/BufferReader.hpp>

#include <cstddef>

/* ************************************************************************** */
class EndRule : public Rule
{
public:
  explicit EndRule(ft::shared_ptr<Rule> rule);
  ~EndRule() {};

  bool matches();
  void reset();
  void setBufferReader(BufferReader* bufferReader);
  void setResultMap(ResultMap* results);

private:
  EndRule();
  EndRule(const EndRule& other);
  EndRule& operator=(const EndRule& other);

  ft::shared_ptr<Rule> _rule;
  std::size_t _pos;
};

#endif
