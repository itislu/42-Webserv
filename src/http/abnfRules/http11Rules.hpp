#pragma once
#ifndef HTTP11_RULES_HPP
#define HTTP11_RULES_HPP

#include <libftpp/memory.hpp>
#include <utils/abnfRules/AlternativeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

ft::shared_ptr<AlternativeRule> requestTargetRule();
ft::shared_ptr<SequenceRule> originFormRule();
ft::shared_ptr<RepetitionRule> absolutePathRule();
ft::shared_ptr<SequenceRule> absoluteFormRule();

#endif
