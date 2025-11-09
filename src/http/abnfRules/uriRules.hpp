#pragma once
#ifndef URI_RULES_HPP
#define URI_RULES_HPP

#include <libftpp/memory.hpp>
#include <utils/abnfRules/AlternativeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

ft::shared_ptr<SequenceRule> uriRule();
ft::shared_ptr<AlternativeRule> hierPartRule();
ft::shared_ptr<SequenceRule> absoluteUriRule();
ft::shared_ptr<SequenceRule> schemeRule();
ft::shared_ptr<SequenceRule> authorityRule();
ft::shared_ptr<RepetitionRule> userinfoRule();
ft::shared_ptr<AlternativeRule> hostRule();
ft::shared_ptr<RepetitionRule> portRule();
ft::shared_ptr<SequenceRule> ipLiteralRule();
ft::shared_ptr<SequenceRule> ipvFutureRule();
ft::shared_ptr<AlternativeRule> ipv6AddressRule();
ft::shared_ptr<RepetitionRule> h16Rule();
ft::shared_ptr<AlternativeRule> ls32Rule();
ft::shared_ptr<SequenceRule> ipv4AddressRule();
ft::shared_ptr<AlternativeRule> decOctetRule();
ft::shared_ptr<RepetitionRule> regNameRule();
ft::shared_ptr<AlternativeRule> pathRule();
ft::shared_ptr<RepetitionRule> pathAbEmptyRule();
ft::shared_ptr<SequenceRule> pathAbsoluteRule();
ft::shared_ptr<SequenceRule> pathNoSchemeRule();
ft::shared_ptr<SequenceRule> pathRootlessRule();
ft::shared_ptr<RepetitionRule> pathEmptyRule();
ft::shared_ptr<RepetitionRule> segmentRule();
ft::shared_ptr<RepetitionRule> segmentNzRule();
ft::shared_ptr<RepetitionRule> segmentNzNcRule();
ft::shared_ptr<AlternativeRule> pcharRule();
ft::shared_ptr<RepetitionRule> queryRule();
ft::shared_ptr<RepetitionRule> fragmentRule();
ft::shared_ptr<SequenceRule> pctRule();

#endif
