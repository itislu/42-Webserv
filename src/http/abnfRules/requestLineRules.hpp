#pragma once
#ifndef REQUEST_LINE_RULES_HPP
#define REQUEST_LINE_RULES_HPP

#include <utils/abnfRules/SequenceRule.hpp>

SequenceRule* authorityPartRule();
SequenceRule* pathPartRule();
SequenceRule* schemePartRule();
SequenceRule* queryPartRule();
SequenceRule* fragmentPartRule();

#endif // REQUEST_LINE_RULES_HPP
