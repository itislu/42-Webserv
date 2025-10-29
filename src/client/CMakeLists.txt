# **************************************************************************** #
# CLIENT MODULE
# **************************************************************************** #

set(LIB_NAME client)

add_library(${LIB_NAME} STATIC
  Client.cpp
  ClientManager.cpp
  TimeStamp.cpp
)

target_link_libraries(${LIB_NAME} 
  PRIVATE
  ftpp
  http
)
