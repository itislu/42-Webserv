# **************************************************************************** #
# UTILS MODULE
# **************************************************************************** #

set(LIB_NAME http)

add_library(${LIB_NAME} STATIC
  states/IState.cpp
  states/ReadStartLine.cpp
  Request.cpp
  Uri.cpp
)

target_link_libraries(${LIB_NAME} 
  PRIVATE
  ftpp
)
