# **************************************************************************** #
# SERVER MODULE
# **************************************************************************** #

set(LIB_NAME server)

add_library(${LIB_NAME} STATIC
  Server.cpp
  ServerManager.cpp
)

target_link_libraries(${LIB_NAME} 
  PRIVATE
  client
  config
  event
  ftpp
  socket
)
