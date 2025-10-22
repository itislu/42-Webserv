#pragma once
#ifndef RULE_FACTORIES_HPP
#define RULE_FACTORIES_HPP

#include <utils/abnfRules/AlternativRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

SequenceRule* uriRule();
AlternativRule* hierPartRule();
SequenceRule* schemeRule();
SequenceRule* authorityRule();
RepetitionRule* userinfoRule();
AlternativRule* hostRule();
RepetitionRule* portRule();
SequenceRule* ipLiteralRule();
SequenceRule* ipvFutureRule();
AlternativRule* ipv6AddressRule();
RepetitionRule* h16Rule();
AlternativRule* ls32Rule();
SequenceRule* ipv4AddressRule();
AlternativRule* decOctetRule();
RepetitionRule* regNameRule();
AlternativRule* pathRule();
RepetitionRule* pathAbEmptyRule();
SequenceRule* pathAbsoluteRule();
SequenceRule* pathNoScheme();
SequenceRule* pathRootless();
RepetitionRule* segmentRule();
RepetitionRule* segmentNzRule();
RepetitionRule* segmentNzNcRule();
AlternativRule* pcharRule();
RepetitionRule* queryRule();
RepetitionRule* fragmentRule();
SequenceRule* pctRule();

#endif // RULE_FACTORIES_HPP
