# **************************************************************************** #
# WEB SERVER
# **************************************************************************** #

cmake_minimum_required(VERSION 3.14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

find_program(CXX_COMPILER "c++")
set(CMAKE_CXX_COMPILER "${CXX_COMPILER}")
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# **************************************************************************** #
# PROJECT
project(webserv VERSION 1.0)

add_executable(${PROJECT_NAME}
  src/main.cpp
)

include_directories(PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  ftpp
  utils
  server
  client
  socket
  config
  event
)



# **************************************************************************** #
# LIBRARIES
add_subdirectory(lib/libftpp)


# **************************************************************************** #
# MODULES
add_subdirectory(src/client)
add_subdirectory(src/config)
add_subdirectory(src/event)
add_subdirectory(src/server)
add_subdirectory(src/socket)
add_subdirectory(src/utils)


# **************************************************************************** #
# TESTS
enable_testing()
add_subdirectory(tests)
