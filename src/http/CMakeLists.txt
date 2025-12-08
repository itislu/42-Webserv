# **************************************************************************** #
# HTTP MODULE
# **************************************************************************** #

set(LIB_NAME http)

add_library(${LIB_NAME} STATIC
  abnfRules/generalRules.cpp
  abnfRules/headerLineRules.cpp
  abnfRules/http11Rules.cpp
  abnfRules/headerRules.cpp
  abnfRules/requestLineRules.cpp
  abnfRules/ruleIds.cpp
  abnfRules/ruleUtils.cpp
  abnfRules/uriRules.cpp
  
  states/prepareResponse/HandleDelete.cpp
  states/prepareResponse/HandleError.cpp
  states/prepareResponse/HandleGet.cpp
  states/prepareResponse/HandlePost.cpp
  states/prepareResponse/PrepareResponse.cpp
  states/readBody/ReadBody.cpp
  states/readHeaderLines/ReadHeaderLines.cpp
  states/readHeaderLines/RequestHeaderValidator.cpp
  states/readRequestLine/ParseMethod.cpp
  states/readRequestLine/ParseUri.cpp
  states/readRequestLine/ParseVersion.cpp
  states/readRequestLine/ReadRequestLine.cpp
  states/startCgi/StartCgi.cpp
  states/validateRequest/ValidateDelete.cpp
  states/validateRequest/ValidateGet.cpp
  states/validateRequest/ValidatePost.cpp
  states/validateRequest/ValidateRequest.cpp
  states/waitForCgi/WaitForCgi.cpp
  states/writeBody/WriteBody.cpp
  states/writeHeaderLines/WriteHeaderLines.cpp
  states/writeStatusLine/WriteStatusLine.cpp
  statesCgi/executeCgi/ExecuteCgi.cpp
  statesCgi/processCgiResponse/ProcessCgiResponse.cpp
  
  utils/BaseHeaderValidator.cpp
  utils/HeaderParser.cpp

  Authority.cpp
  CgiContext.cpp
  Headers.cpp
  headerUtils.cpp
  http.cpp
  Request.cpp
  Response.cpp
  Resource.cpp
  StatusCode.cpp
  Uri.cpp
)

target_link_libraries(${LIB_NAME} 
  PRIVATE
  config
  ftpp
  utils
)
