#pragma once
#ifndef LITERAL_RULE_HPP
#define LITERAL_RULE_HPP

#include "Rule.hpp"

#include <utils/buffer/IInBuffer.hpp>

#include <cstddef>
#include <string>

/* ************************************************************************** */
class LiteralRule : public Rule
{
public:
  explicit LiteralRule(const char* literal);
  ~LiteralRule();

  bool matches();
  void reset();
  void setBufferReader(IInBuffer* bufferReader);
  void setResultMap(ResultMap* results);

private:
  LiteralRule();
  LiteralRule(const LiteralRule& other);
  LiteralRule& operator=(const LiteralRule& other);

  std::size_t _pos;
  std::string _literal;
};

#endif // LiteralRule_HPP
