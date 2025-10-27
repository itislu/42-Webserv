# **************************************************************************** #
# EVENT MODULE
# **************************************************************************** #

set(LIB_NAME event)

add_library(${LIB_NAME} STATIC
  EventManager.cpp
)

target_link_libraries(${LIB_NAME} 
  PRIVATE
  client
  config
  ftpp
  socket
)
