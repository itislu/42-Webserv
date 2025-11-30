# **************************************************************************** #
# TESTS
# **************************************************************************** #

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")



# **************************************************************************** #
# Download & add googletest
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)

FetchContent_MakeAvailable(googletest)



# **************************************************************************** #
# Set valgrind options
set(VALGRIND_COMMAND_OPTIONS "--errors-for-leak-kinds=all \
                              --leak-check=full \
                              --num-callers=50 \
                              --show-leak-kinds=all \
                              --trace-children=yes \
                              --track-origins=yes \
                              --track-fds=all"
)
include(CTest)



# **************************************************************************** #
# Function to add test
function(add_my_test test_name)
  add_executable(${test_name} ${ARGN})

  target_compile_definitions(${test_name}
    PRIVATE ASSETS_PATH="${PROJECT_SOURCE_DIR}/tests/assets/"
  )

  target_link_libraries(${test_name}
    PRIVATE
    GTest::gtest
    pthread
    ftpp
    client
    socket
    config
    event
    http
    server
    utils
    config
  )

  add_test(NAME ${PROJECT_NAME}_${test_name} COMMAND ${test_name})
endfunction()


# **************************************************************************** #
# Tester

# utils
add_my_test(utilsTester
  ${CMAKE_CURRENT_SOURCE_DIR}/utils_tests/utilsTester.cpp
)

add_my_test(fileBufferTester
  ${CMAKE_CURRENT_SOURCE_DIR}/utils_tests/fileBufferTester.cpp
)

add_my_test(smartBufferTester
  ${CMAKE_CURRENT_SOURCE_DIR}/utils_tests/smartBufferTester.cpp
)

# http

# http/parser
add_my_test(http11RulesTester
  ${CMAKE_CURRENT_SOURCE_DIR}/http/abnfRules/http11RulesTester.cpp
)

add_my_test(headerLineRulesTester
  ${CMAKE_CURRENT_SOURCE_DIR}/http/abnfRules/headerLineRulesTester.cpp
)

add_my_test(requestLineRulesTester
  ${CMAKE_CURRENT_SOURCE_DIR}/http/abnfRules/requestLineRulesTester.cpp
)

add_my_test(uriRulesTester
  ${CMAKE_CURRENT_SOURCE_DIR}/http/abnfRules/uriRulesTester.cpp
)

# http/states

add_my_test(readHeaderLinesTester
  ${CMAKE_CURRENT_SOURCE_DIR}/http/states/readHeaderLinesTester.cpp
)

add_my_test(readRequestLineTester
  ${CMAKE_CURRENT_SOURCE_DIR}/http/states/readRequestLineTester.cpp
)

add_my_test(requestTester
  ${CMAKE_CURRENT_SOURCE_DIR}/http/states/requestTester.cpp
)

# config

add_my_test(configParserTester
  ${CMAKE_CURRENT_SOURCE_DIR}/config/configParserTester.cpp
)

# locations

add_my_test(locationMatchTester
  ${CMAKE_CURRENT_SOURCE_DIR}/locations/locationMatchTester.cpp
)
