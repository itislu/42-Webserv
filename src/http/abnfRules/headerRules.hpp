#pragma once
#ifndef HEADER_RULES_HPP
#define HEADER_RULES_HPP

#include <libftpp/memory.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

ft::shared_ptr<SequenceRule> fieldLineWrapper(
  ft::shared_ptr<Rule> fieldValueRule);

ft::shared_ptr<SequenceRule> hostHeaderRule();
ft::shared_ptr<RepetitionRule> contentLengthRule();
ft::shared_ptr<SequenceRule> transferEncodingRule();
ft::shared_ptr<SequenceRule> transferCodingRule();
ft::shared_ptr<SequenceRule> transferParameterRule();

#endif
