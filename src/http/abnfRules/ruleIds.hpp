#pragma once
#ifndef RULE_IDS_HPP
#define RULE_IDS_HPP

#include <string>

enum HttpRuleId
{
  Undefined = -1,
  EndOfRule,

  // GeneralRules
  EndOfLine,

  // RequestLineIds
  SchemePart,
  Scheme,
  AuthorityPart,
  UserInfo,
  Host,
  Port,
  PathPart,
  PathAbEmpty,
  QueryPart,
  FragmentPart,
  OriginForm,
  AbsoluteForm,
  Query,
  Fragment,
  AbsolutePath,
  HierPart,
  HierPartPathAbEmpty,
  HierPartPathAbsolute,
  HierPartPathRootless,
  HierPartPathEmpty,

  // HeaderLineIds
  FieldLinePart
};

std::string ruleIdtoString(HttpRuleId rid);

#endif // RULE_IDS_HPP
