#pragma once
#ifndef RULE_HPP
#define RULE_HPP

#include <utils/BufferReader.hpp>
#include <utils/abnfRules/RuleResult.hpp>

#include <map>

/* ************************************************************************** */
class Rule
{
public:
  typedef int RuleId;
  typedef std::map<RuleId, RuleResult> ResultMap;

  Rule();
  virtual ~Rule();

  virtual bool matches() = 0;
  virtual void reset() = 0;
  virtual void setBufferReader(BufferReader* bufferReader) = 0;
  virtual void setResultMap(ResultMap* results) = 0;

  bool end() const;
  void setRuleId(RuleId ruleId);

  void setDebugTag(const char* str);
  void setDebugPrintIndent(int value);
  int getDebugPrintIndent() const;
  void setDebugMatchReason(const char* reason);
  static bool debugPrint;

protected:
  void setEndOfRule(bool value);
  BufferReader* getBuffReader();

  void setStartPos(long pos);
  long getStartPos() const;
  void setEndPos(long pos);
  long getEndPos() const;

  void rewindToStartPos();
  void moveToEndPos();

  void addRuleResult(bool matches);

  void debugPrintRuleEntry();
  void debugPrintMatchStatus(bool matches);

private:
  Rule(const Rule& other);
  Rule& operator=(const Rule& other);

  bool _endOfRule;
  BufferReader* _buffReader;

  long _startPos;
  long _endPos;

  static const RuleId _ruleIdUndefined = -1;
  RuleId _ruleId;
  ResultMap* _results;

  static const int _debugInitPrintIndent = 20;
  int _debugPrintIndent;
  const char* _debugTag;
  const char* _debugMatchReason;
};

#endif // RULE_HPP
