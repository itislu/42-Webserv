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
# TESTS
# **************************************************************************** #

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")


include(CTest)
set(MEMORYCHECK_COMMAND valgrind)
set(MEMORYCHECK_COMMAND_OPTIONS "--errors-for-leak-kinds=all \
                                 --leak-check=full \
                                 --show-error-list=yes \
                                 --show-leak-kinds=all \
                                 --suppressions=minishell.supp \
                                 --trace-children=yes \
                                 --track-origins=yes \
                                 --track-fds=yes"
)


find_package(GTest REQUIRED)



# **************************************************************************** #
# function to add test
function(add_my_test test_name)
  add_executable(${test_name} ${ARGN})

  target_compile_definitions(${test_name}
    PRIVATE ASSETS_PATH="${PROJECT_SOURCE_DIR}/tests/assets/"
  )

  target_link_libraries(${test_name}
    PRIVATE
    GTest::gtest
    pthread
    utils
    ftpp
  )

  add_test(NAME ${PROJECT_NAME}_${test_name} COMMAND ${test_name})
endfunction()


# **************************************************************************** #
# Tester

add_my_test(utilsTester
  ${CMAKE_CURRENT_SOURCE_DIR}/utils_tests/utilsTester.cpp
)
