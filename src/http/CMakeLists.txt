# **************************************************************************** #
# HTTP MODULE
# **************************************************************************** #

set(LIB_NAME http)

add_library(${LIB_NAME} STATIC
  abnfRules/requestLineRules.cpp
  abnfRules/uriRules.cpp
  http.cpp
)

target_link_libraries(${LIB_NAME} 
  PRIVATE
  ftpp
  utils
)
