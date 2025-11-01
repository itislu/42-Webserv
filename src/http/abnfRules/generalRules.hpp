#pragma once
#ifndef GENERAL_RULES_HPP
#define GENERAL_RULES_HPP

#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

SequenceRule* endOfLineRule();
RepetitionRule* owsRule();
RepetitionRule* rwsRule();

#endif
