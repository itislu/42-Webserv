#pragma once
#ifndef REQUEST_LINE_RULES_HPP
#define REQUEST_LINE_RULES_HPP

#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

SequenceRule* authorityPartRule();
SequenceRule* pathPartRule();
SequenceRule* schemePartRule();
SequenceRule* endOfRequestLineRule();
RepetitionRule* whitespaceRule(int min, int max);

#endif // REQUEST_LINE_RULES_HPP
