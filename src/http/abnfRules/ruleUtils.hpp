#pragma once
#ifndef RULE_UTILS_HPP
#define RULE_UTILS_HPP

#include <utils/abnfRules/Rule.hpp>

#include <string>

void printRuleResults(const Rule::ResultMap& results);
bool isValidString(Rule& rule, const std::string& value);

#endif
