#pragma once
#ifndef RULE_IDS_HPP
#define RULE_IDS_HPP

enum RuleId
{
  EndOfRule,

  // GeneralRules
  EndOfLine,

  // RequestLineIds
  SchemePart,
  AuthorityPart,
  PathPart,
  QueryPart,
  FragmentPart,

  // HeaderLineIds
  FieldLinePart
};

#endif // RULE_IDS_HPP
