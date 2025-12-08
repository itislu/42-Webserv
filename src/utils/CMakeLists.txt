# **************************************************************************** #
# UTILS MODULE
# **************************************************************************** #

set(LIB_NAME utils)

add_library(${LIB_NAME} STATIC
  abnfRules/AlternativeRule.cpp
  abnfRules/EndRule.cpp
  abnfRules/LiteralRule.cpp
  abnfRules/RangeRule.cpp
  abnfRules/RepetitionRule.cpp
  abnfRules/Rule.cpp
  abnfRules/RuleResult.cpp
  abnfRules/SequenceRule.cpp

  buffer/BufferQueue.cpp
  buffer/FileBuffer.cpp
  buffer/IBuffer.cpp
  buffer/IInBuffer.cpp
  buffer/IOutBuffer.cpp
  buffer/MemoryBuffer.cpp
  buffer/SmartBuffer.cpp
  buffer/StaticFileBuffer.cpp

  logger/Logger.cpp

  BufferReader.cpp
  convert.cpp
  fileUtils.cpp
  Pipe.cpp
  printUtils.cpp
)

target_link_libraries(${LIB_NAME} 
  PRIVATE
  ftpp
)
