# **************************************************************************** #
# HTTP MODULE
# **************************************************************************** #

set(LIB_NAME http)

add_library(${LIB_NAME} STATIC
  abnfRules/generalRules.cpp
  abnfRules/http11Rules.cpp
  abnfRules/requestLineRules.cpp
  abnfRules/ruleIds.cpp
  abnfRules/ruleUtils.cpp
  abnfRules/uriRules.cpp
  abnfRules/headerLineRules.cpp
  states/prepareResponse/HandleDelete.cpp
  states/prepareResponse/HandleError.cpp
  states/prepareResponse/HandleGet.cpp
  states/prepareResponse/HandlePost.cpp
  states/prepareResponse/PrepareResponse.cpp
  states/readBody/ReadBody.cpp
  states/readRequestLine/ParseMethod.cpp
  states/readRequestLine/ParseUri.cpp
  states/readRequestLine/ParseVersion.cpp
  states/readRequestLine/ReadRequestLine.cpp
  states/readRequestLine/ValidateRequest.cpp
  states/readHeaderLines/ReadHeaderLines.cpp
  states/writeBody/WriteBody.cpp
  states/writeHeaderLines/WriteHeaderLines.cpp
  states/writeStatusLine/WriteStatusLine.cpp
  Authority.cpp
  Headers.cpp
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
