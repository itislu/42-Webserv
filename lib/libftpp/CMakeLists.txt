# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    CMakeLists.txt                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dpotsch <poetschdavid@gmail.com>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/07 10:39:24 by dpotsch           #+#    #+#              #
#    Updated: 2025/09/10 12:29:00 by dpotsch          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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
