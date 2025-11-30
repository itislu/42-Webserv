#pragma once
#ifndef RULE_HPP
#define RULE_HPP

#include <utils/abnfRules/RuleResult.hpp>
#include <utils/buffer/IInBuffer.hpp>

#include <cstddef>
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
  virtual void setBufferReader(IInBuffer* bufferReader) = 0;
  virtual void setResultMap(ResultMap* results) = 0;

  bool reachedEnd() const;
  void setRuleId(RuleId ruleId);

  void setDebugTag(const char* str);
  const char* getDebugTag() const;
  void setDebugMatchReason(const char* reason);
  static bool debugPrint;

protected:
  void setReachedEnd(bool value);
  IInBuffer* getBuffReader();

  void setStartPos(std::size_t pos);
  std::size_t getStartPos() const;
  void setEndPos(std::size_t pos);
  std::size_t getEndPos() const;

  void rewindToStartPos();
  void moveToEndPos();

  void addRuleResult(bool matches);

  void debugPrintRuleEntry();
  void debugPrintMatchStatus(bool matches);

  bool bufferReachedEnd();

private:
  Rule(const Rule& other);
  Rule& operator=(const Rule& other);

  bool _reachedEnd;
  IInBuffer* _buffReader;

  std::size_t _startPos;
  std::size_t _endPos;

  static const RuleId _ruleIdUndefined = -1;
  RuleId _ruleId;
  ResultMap* _results;

  static const int _debugInitPrintIndent = 20;
  const char* _debugTag;
  const char* _debugMatchReason;
};

#endif
