#pragma once
#ifndef GENERAL_RULES_HPP
#define GENERAL_RULES_HPP

#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>

LiteralRule* endOfLineRule();
RepetitionRule* owsRule();
RepetitionRule* rwsRule();

#endif
