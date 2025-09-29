# **************************************************************************** #
# FORTY TWO CPP LIBRARY
# **************************************************************************** #

set(LIB_NAME ftpp)

add_library(${LIB_NAME} STATIC
  libftpp/trim/trim.cpp
)

set_target_properties(${LIB_NAME} PROPERTIES
  CXX_STANDARD 98
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_compile_options(${LIB_NAME}
  PRIVATE -Wall -Wextra -Werror -Wpedantic
)

target_include_directories(${LIB_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
