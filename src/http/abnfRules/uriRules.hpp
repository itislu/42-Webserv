#pragma once
#ifndef URI_RULES_HPP
#define URI_RULES_HPP

#include <utils/abnfRules/AlternativeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

SequenceRule* uriRule();
AlternativeRule* hierPartRule();
SequenceRule* schemeRule();
SequenceRule* authorityRule();
RepetitionRule* userinfoRule();
AlternativeRule* hostRule();
RepetitionRule* portRule();
SequenceRule* ipLiteralRule();
SequenceRule* ipvFutureRule();
AlternativeRule* ipv6AddressRule();
RepetitionRule* h16Rule();
AlternativeRule* ls32Rule();
SequenceRule* ipv4AddressRule();
AlternativeRule* decOctetRule();
RepetitionRule* regNameRule();
AlternativeRule* pathRule();
RepetitionRule* pathAbEmptyRule();
SequenceRule* pathAbsoluteRule();
SequenceRule* pathNoScheme();
SequenceRule* pathRootless();
RepetitionRule* pathEmptyRule();
RepetitionRule* segmentRule();
RepetitionRule* segmentNzRule();
RepetitionRule* segmentNzNcRule();
AlternativeRule* pcharRule();
RepetitionRule* queryRule();
RepetitionRule* fragmentRule();
SequenceRule* pctRule();

#endif
