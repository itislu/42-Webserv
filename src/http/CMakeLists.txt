# **************************************************************************** #
# HTTP MODULE
# **************************************************************************** #

set(LIB_NAME http)

add_library(${LIB_NAME} STATIC
  abnfRules/generalRules.cpp
  abnfRules/requestLineRules.cpp
  abnfRules/uriRules.cpp
  abnfRules/headerLineRules.cpp
  states/readStartLine/ParseMethod.cpp
  states/readStartLine/ParseUri.cpp
  states/readStartLine/ParseVersion.cpp
  states/readStartLine/ReadStartLine.cpp
  states/readHeaderLines/ReadHeaderLines.cpp
  states/writeStatusLine/WriteStatusLine.cpp
  http.cpp
  Request.cpp
  Response.cpp
  StatusCode.cpp
  Uri.cpp
)

target_link_libraries(${LIB_NAME} 
  PRIVATE
  ftpp
  utils
)
