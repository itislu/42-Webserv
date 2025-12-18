# **************************************************************************** #
# EVENT MODULE
# **************************************************************************** #

set(LIB_NAME event)

add_library(${LIB_NAME} STATIC
  CgiReadEventHandler.cpp
  CgiWriteEventHandler.cpp
  ClientEventHandler.cpp
  EventHandler.cpp
  EventManager.cpp
)

target_link_libraries(${LIB_NAME} 
  PRIVATE
  client
  config
  ftpp
  socket
)
