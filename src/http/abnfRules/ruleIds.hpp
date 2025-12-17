#pragma once
#ifndef RULE_IDS_HPP
#define RULE_IDS_HPP

#include <string>

enum HttpRuleId
{
  EndOfRule = -1,
  Undefined,

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
  FieldLinePart,

  // Body
  ChunkInfo,
  ChunkSize,
  ChunkExt
};

std::string ruleIdtoString(HttpRuleId rid);

#endif // RULE_IDS_HPP
