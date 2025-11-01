#pragma once
#ifndef HEADER_LINE_RULES_HPP
#define HEADER_LINE_RULES_HPP

#include <libftpp/memory.hpp>
#include <utils/abnfRules/AlternativeRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

ft::unique_ptr<SequenceRule> fieldLinePartRule();
SequenceRule* fieldLineRule();
RepetitionRule* fieldNameRule();
RepetitionRule* fieldValueRule();
SequenceRule* fieldContentRule();
AlternativeRule* fieldVcharRule();
RangeRule* obsTextRule();

#endif
