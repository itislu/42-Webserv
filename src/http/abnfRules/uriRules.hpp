#pragma once
#ifndef URI_RULES_HPP
#define URI_RULES_HPP

#include <libftpp/memory.hpp>
#include <utils/abnfRules/AlternativeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

ft::unique_ptr<SequenceRule> uriRule();
ft::unique_ptr<AlternativeRule> hierPartRule();
ft::unique_ptr<SequenceRule> schemeRule();
ft::unique_ptr<SequenceRule> authorityRule();
ft::unique_ptr<RepetitionRule> userinfoRule();
ft::unique_ptr<AlternativeRule> hostRule();
ft::unique_ptr<RepetitionRule> portRule();
ft::unique_ptr<SequenceRule> ipLiteralRule();
ft::unique_ptr<SequenceRule> ipvFutureRule();
ft::unique_ptr<AlternativeRule> ipv6AddressRule();
ft::unique_ptr<RepetitionRule> h16Rule();
ft::unique_ptr<AlternativeRule> ls32Rule();
ft::unique_ptr<SequenceRule> ipv4AddressRule();
ft::unique_ptr<AlternativeRule> decOctetRule();
ft::unique_ptr<RepetitionRule> regNameRule();
ft::unique_ptr<AlternativeRule> pathRule();
ft::unique_ptr<RepetitionRule> pathAbEmptyRule();
ft::unique_ptr<SequenceRule> pathAbsoluteRule();
ft::unique_ptr<SequenceRule> pathNoSchemeRule();
ft::unique_ptr<SequenceRule> pathRootlessRule();
ft::unique_ptr<RepetitionRule> pathEmptyRule();
ft::unique_ptr<RepetitionRule> segmentRule();
ft::unique_ptr<RepetitionRule> segmentNzRule();
ft::unique_ptr<RepetitionRule> segmentNzNcRule();
ft::unique_ptr<AlternativeRule> pcharRule();
ft::unique_ptr<RepetitionRule> queryRule();
ft::unique_ptr<RepetitionRule> fragmentRule();
ft::unique_ptr<SequenceRule> pctRule();

#endif
