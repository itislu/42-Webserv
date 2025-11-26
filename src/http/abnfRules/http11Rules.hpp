#pragma once
#ifndef HTTP11_RULES_HPP
#define HTTP11_RULES_HPP

#include <libftpp/memory.hpp>
#include <utils/abnfRules/AlternativeRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

ft::shared_ptr<AlternativeRule> requestTargetRule();
ft::shared_ptr<SequenceRule> originFormRule();
ft::shared_ptr<RepetitionRule> absolutePathRule();
ft::shared_ptr<SequenceRule> absoluteFormRule();
ft::shared_ptr<RangeRule> qdtextRule();
ft::shared_ptr<SequenceRule> quotedPairRule();
ft::shared_ptr<SequenceRule> quotedStringRule();
ft::shared_ptr<SequenceRule> lastChunkRule();
ft::shared_ptr<RepetitionRule> chunkExtRule();
ft::shared_ptr<SequenceRule> chunkInfoRule();
ft::shared_ptr<RepetitionRule> chunkSizeRule();
ft::shared_ptr<RepetitionRule> chunkExtNameRule();
ft::shared_ptr<AlternativeRule> chunkExtValRule();
#endif
