# **************************************************************************** #
# UTILS MODULE
# **************************************************************************** #

set(LIB_NAME utils)

add_library(${LIB_NAME} STATIC
  abnfRules/AlternativeRule.cpp
  abnfRules/LiteralRule.cpp
  abnfRules/RangeRule.cpp
  abnfRules/RepetitionRule.cpp
  abnfRules/Rule.cpp
  abnfRules/RuleResult.cpp
  abnfRules/SequenceRule.cpp
  Buffer.cpp
  BufferReader.cpp
)

target_link_libraries(${LIB_NAME} 
  PRIVATE
  ftpp
)
