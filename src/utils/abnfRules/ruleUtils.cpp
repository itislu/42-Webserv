#include "ruleUtils.hpp"

#include <http/abnfRules/ruleIds.hpp>
#include <utils/abnfRules/Rule.hpp>

#include <iomanip>
#include <iostream>

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
