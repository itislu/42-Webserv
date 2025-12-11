#pragma once
#ifndef GENERAL_RULES_HPP
#define GENERAL_RULES_HPP

#include <libftpp/memory.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

ft::shared_ptr<LiteralRule> endOfLineRule();
ft::shared_ptr<RepetitionRule> owsRule();
ft::shared_ptr<RepetitionRule> rwsRule();
ft::shared_ptr<RepetitionRule> bwsRule();
ft::shared_ptr<SequenceRule> listRule(ft::shared_ptr<Rule> elementRule);

#endif
