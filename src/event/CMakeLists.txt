# **************************************************************************** #
# EVENT MODULE
# **************************************************************************** #

set(LIB_NAME event)

add_library(${LIB_NAME} STATIC
  EventManager.cpp
  TimeoutManager.cpp
  TimeStamp.cpp
)

target_link_libraries(${LIB_NAME} 
  PRIVATE
  ftpp
  utils
)
