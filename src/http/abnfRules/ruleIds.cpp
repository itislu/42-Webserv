#include "ruleIds.hpp"

#include <libftpp/utility.hpp>

#include <string>

std::string ruleIdtoString(HttpRuleId rid)
{
  switch (rid) {
    case Undefined:
      return "Undefined";
    case EndOfRule:
      return "EndOfRule";
    case EndOfLine:
      return "EndOfLine";
    case SchemePart:
      return "SchemePart";
    case Scheme:
      return "Scheme";
    case AuthorityPart:
      return "AuthorityPart";
    case UserInfo:
      return "UserInfo";
    case Host:
      return "Host";
    case Port:
      return "Port";
    case PathPart:
      return "PathPart";
    case PathAbEmpty:
      return "PathAbEmpty";
    case QueryPart:
      return "QueryPart";
    case FragmentPart:
      return "FragmentPart";
    case OriginForm:
      return "OriginForm";
    case AbsoluteForm:
      return "AbsoluteForm";
    case Query:
      return "Query";
    case Fragment:
      return "Fragment";
    case AbsolutePath:
      return "AbsolutePath";
    case HierPart:
      return "HierPart";
    case HierPartPathAbEmpty:
      return "HierPartPathAbEmpty";
    case HierPartPathAbsolute:
      return "HierPartPathAbsolute";
    case HierPartPathRootless:
      return "HierPartPathRootless";
    case HierPartPathEmpty:
      return "HierPartPathEmpty";
    case FieldLinePart:
      return "FieldLinePart";
    case ChunkInfo:
      return "ChunkInfo";
    case ChunkSize:
      return "ChunkSize";
    case ChunkExt:
      return "ChunkExt";
  }
  FT_UNREACHABLE();
}
