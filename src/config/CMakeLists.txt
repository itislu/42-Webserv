# **************************************************************************** #
# CONFIG MODULE
# **************************************************************************** #

set(LIB_NAME utils)

add_library(${LIB_NAME} STATIC
  .cpp
)

target_link_libraries(${LIB_NAME} 
  PRIVATE
  ftpp
)
