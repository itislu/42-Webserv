# **************************************************************************** #
# SOCKET MODULE
# **************************************************************************** #

set(LIB_NAME socket)

add_library(${LIB_NAME} STATIC
  Socket.cpp
  AutoFd.cpp
)

target_link_libraries(${LIB_NAME} 
  PRIVATE
  ftpp
)
