# **************************************************************************** #
# HTTP MODULE
# **************************************************************************** #

set(LIB_NAME http)

add_library(${LIB_NAME} STATIC
  abnfRules/requestLineRules.cpp
  abnfRules/uriRules.cpp
  states/readStartLine/ParseMethod.cpp
  states/readStartLine/ParseUri.cpp
  states/readStartLine/ParseVersion.cpp
  states/readStartLine/ReadStartLine.cpp
  http.cpp
  Request.cpp
  Uri.cpp
)

target_link_libraries(${LIB_NAME} 
  PRIVATE
  ftpp
)
