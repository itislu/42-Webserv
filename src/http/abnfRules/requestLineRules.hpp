#pragma once
#ifndef REQUEST_LINE_RULES_HPP
#define REQUEST_LINE_RULES_HPP

#include <libftpp/memory.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

ft::unique_ptr<SequenceRule> authorityPartRule();
ft::unique_ptr<SequenceRule> pathPartRule();
ft::unique_ptr<SequenceRule> schemePartRule();
ft::unique_ptr<SequenceRule> queryPartRule();
ft::unique_ptr<SequenceRule> fragmentPartRule();

#endif // REQUEST_LINE_RULES_HPP
