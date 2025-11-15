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
                       const Rule::ResultMap& resultMap,
                       const Buffer& buffer) const
{
  for (typename Setters::const_iterator setterIter = _setters.begin();
       setterIter != _setters.end();
       ++setterIter) {
    Rule::ResultMap::const_iterator resultIter =
      resultMap.find(setterIter->first);
    if (resultIter != resultMap.end()) {
      ft::optional<std::string> str = _getString(resultIter->second, buffer);
      if (str.has_value()) {
        FuncPtr fPtr = setterIter->second;
        (obj.*fPtr)(*str);
      }
    }
  }
}

/* ************************************************************************** */
// PRIVATE

/**
 * example: http://
 * result 'Scheme' whould be
 * start = -1
 * end = 3
 */
template<typename T>
ft::optional<std::string> Extractor<T>::_getString(const RuleResult& result,
                                                   const Buffer& buffer)
{
  const long start = result.getStart() + 1;
  const long end = result.getEnd() + 1;

  if (start >= end || start < 0) {
    return ft::nullopt;
  }
  return buffer.getString(start, end);
}
