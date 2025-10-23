#pragma once
#ifndef RANGE_RULE_HPP
#define RANGE_RULE_HPP

#include "Rule.hpp"

#include "utils/BufferReader.hpp"

/* ************************************************************************** */
class RangeRule : public Rule
{
public:
  explicit RangeRule(const char* range);
  explicit RangeRule(int (*func)(int));
  ~RangeRule();

  bool matches();
  void reset();
  void setBufferReader(BufferReader* bufferReader);
  void setResultMap(ResultMap* results);

private:
  enum RangeType
  {
    Func,
    Charset
  };

  struct Range
  {
    union
    {
      int (*func)(int);
      const char* set;
    };
    RangeType type;
  };

  RangeRule();
  RangeRule(const RangeRule& other);
  RangeRule& operator=(const RangeRule& other);

  Range _range;
};

#endif
