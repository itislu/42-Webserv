#pragma once
#ifndef EXTRACTOR_HPP
#include "Extractor.hpp"
#endif

#include <libftpp/optional.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/RuleResult.hpp>
#include <utils/buffer/IBuffer.hpp>
#include <utils/buffer/IInBuffer.hpp>

#include <cstddef>

/* ************************************************************************** */
// PUBLIC

template<typename T>
Extractor<T>& Extractor<T>::addMapItem(Rule::RuleId ruleId, FuncPtr funcPtr)
{
  _setters.push_back(std::make_pair(ruleId, funcPtr));
  return *this;
}

template<typename T>
void Extractor<T>::run(T& obj,
                       const Rule::ResultMap& resultMap,
                       IInBuffer& buffer) const
{
  for (typename Setters::const_iterator setterIter = _setters.begin();
       setterIter != _setters.end();
       ++setterIter) {
    Rule::ResultMap::const_iterator resultIter =
      resultMap.find(setterIter->first);
    if (resultIter != resultMap.end()) {
      OptionStr str = _getString(resultIter->second, buffer);
      if (str.has_value()) {
        FuncPtr fPtr = setterIter->second;
        (obj.*fPtr)(*str);
      }
    }
  }
}

/* ************************************************************************** */
// PRIVATE

template<typename T>
typename Extractor<T>::OptionStr Extractor<T>::_getString(
  const RuleResult& result,
  IInBuffer& buffer)
{
  const std::size_t start = result.getStart();
  const std::size_t end = result.getEnd();
  if (start >= end) {
    return ft::nullopt;
  }
  const std::size_t bytes = end - start;
  const IBuffer::ExpectStr expectStr = buffer.getStr(start, bytes);
  return *expectStr;
}
