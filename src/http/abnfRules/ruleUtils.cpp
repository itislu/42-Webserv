#include "ruleUtils.hpp"

#include <http/abnfRules/ruleIds.hpp>
#include <libftpp/utility.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/buffer/MemoryBuffer.hpp>

#include <iomanip>
#include <iostream>
#include <string>

void printRuleResults(const Rule::ResultMap& results)
{
  for (Rule::ResultMap::const_iterator iter = results.begin();
       iter != results.end();
       ++iter) {
    const int width = 25;
    std::cout << std::setw(width) << std::right;
    std::cout << ruleIdtoString(static_cast<HttpRuleId>(iter->first)) << ":";
    std::cout << std::setw(3) << std::left;
    std::cout << iter->second.getStart() << ", ";
    std::cout << iter->second.getEnd() << "\n";
  }
}

bool isValidString(Rule& rule, const std::string& value)
{
  MemoryBuffer buff(value);
  BufferReader reader = BufferReader();
  reader.init(&buff);
  rule.reset();
  rule.setBufferReader(&reader);
  const bool matches = rule.matches();
  const bool ruleReachedEnd = rule.reachedEnd();
  const bool readerReachedEnd = reader.reachedEnd();

  rule.setBufferReader(FT_NULLPTR);
  return matches && ruleReachedEnd && readerReachedEnd;
}
