#pragma once
#ifndef EXTRACTOR_HPP
#define EXTRACTOR_HPP

#include <libftpp/optional.hpp>
#include <utils/Buffer.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/RuleResult.hpp>

#include <string>
#include <utility>
#include <vector>

/* ************************************************************************** */
template<typename T>
class Extractor
{
public:
  typedef void (T::*FuncPtr)(const std::string&);
  void addMapItem(Rule::RuleId ruleId, FuncPtr funcPtr);
  void run(T& obj,
           const Rule::ResultMap& resultMap,
           const Buffer& buffer) const;

private:
  typedef std::pair<Rule::RuleId, FuncPtr> SetterPair;
  typedef std::vector<SetterPair> Setters; // vector to keep order

  Setters _setters;
  static ft::optional<std::string> _getString(const RuleResult& result,
                                              const Buffer& buffer);
};

#include "Extractor.tpp" // IWYU pragma: export

#endif
