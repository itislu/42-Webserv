#pragma once
#ifndef EXTRACTOR_HPP
#include "Extractor.hpp"
#endif

#include <libftpp/optional.hpp>
#include <utils/Buffer.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/RuleResult.hpp>

#include <string>

/* ************************************************************************** */
// PUBLIC
template<typename T>
void Extractor<T>::addMapItem(Rule::RuleId ruleId, FuncPtr funcPtr)
{
  SetterPair pair;
  pair.first = ruleId;
  pair.second = funcPtr;
  _setters.push_back(pair);
}

template<typename T>
void Extractor<T>::run(T& obj,
                       Rule::ResultMap resultMap,
                       const Buffer& buffer) const
{
  for (typename Setters::const_iterator setterIter = _setters.begin();
       setterIter != _setters.end();
       ++setterIter) {
    Rule::ResultMap::iterator resultIter = resultMap.find(setterIter->first);
    if (resultIter != resultMap.end()) {
      ft::optional<std::string> str = _getString(resultIter->second, buffer);
      FuncPtr fPtr = setterIter->second;
      if (str.has_value()) {
        (obj.*fPtr)(*str);
      }
    }
  }
}

/* ************************************************************************** */
// PRIVATE

template<typename T>
ft::optional<std::string> Extractor<T>::_getString(const RuleResult& result,
                                                   const Buffer& buffer)
{
  long start = result.getStart();
  const long end = result.getEnd();

  if (start >= end) {
    return ft::nullopt;
  }
  start++;
  if (start == -1) {
    start = 0;
  }
  return buffer.getString(start, end + 1);
}
