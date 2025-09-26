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
# WEB SERVER
# **************************************************************************** #

cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")



# **************************************************************************** #
option(ENABLE_UBSAN "Enable Undefined Sanitizer" OFF)
if (ENABLE_UBSAN)
  message(STATUS "sanitizer 'undefined' enabled")
  set(ASAN_FLAGS "-fsanitize=undefined")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ASAN_FLAGS}")
  set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${ASAN_FLAGS}")
endif()



# **************************************************************************** #
# PROJECT
project(webserv VERSION 1.0)


add_executable(${PROJECT_NAME}
  src/main.cpp
)

set(CMAKE_CXX_COMPILER "c++")
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Werror)

include_directories(PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} 
  PRIVATE
  ftpp
  utils
)



# **************************************************************************** #
# LIBRARIES
add_subdirectory(lib/libftpp)



# **************************************************************************** #
# MODULES
add_subdirectory(src/utils)



# **************************************************************************** #
# TESTS
enable_testing()
add_subdirectory(tests)
