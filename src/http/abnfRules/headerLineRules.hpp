#pragma once
#ifndef HEADER_LINE_RULES_HPP
#define HEADER_LINE_RULES_HPP

#include <libftpp/memory.hpp>
#include <utils/abnfRules/AlternativeRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

ft::unique_ptr<SequenceRule> fieldLinePartRule();
ft::shared_ptr<SequenceRule> fieldLineRule();
ft::shared_ptr<RepetitionRule> fieldNameRule();
ft::shared_ptr<RepetitionRule> tokenRule();
ft::shared_ptr<RepetitionRule> fieldValueRule();
ft::shared_ptr<SequenceRule> fieldContentRule();
ft::shared_ptr<AlternativeRule> fieldVcharRule();
ft::shared_ptr<RangeRule> obsTextRule();

#endif
