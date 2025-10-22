#pragma once
#ifndef LiteralRule_HPP
#define LiteralRule_HPP

#include "Rule.hpp"

#include "utils/BufferReader.hpp"

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
  void setBufferReader(BufferReader* bufferReader);
  void setResultMap(ResultMap* results);

private:
  LiteralRule();
  LiteralRule(const LiteralRule& other);
  LiteralRule& operator=(const LiteralRule& other);

  std::size_t _pos;
  std::string _literal;
};

#endif // LiteralRule_HPP
