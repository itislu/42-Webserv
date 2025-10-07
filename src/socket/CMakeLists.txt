# **************************************************************************** #
# SOCKET MODULE
# **************************************************************************** #

set(LIB_NAME server)

add_library(${LIB_NAME} STATIC
  Socket.cpp
)

target_link_libraries(${LIB_NAME} 
  PRIVATE
  ftpp
)
