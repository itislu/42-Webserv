# **************************************************************************** #
# LIBFTPP
# **************************************************************************** #

set(LIB_NAME ftpp)

add_library(${LIB_NAME} STATIC
  libftpp/exception/exception.cpp
  libftpp/expected/bad_expected_access_void.cpp
  libftpp/expected/unexpect_t.cpp
  libftpp/format/log.cpp
  libftpp/memory/shared_ptr_control_base.cpp
  libftpp/numeric/abs_diff.cpp
  libftpp/numeric/arithmetic_exception.cpp
  libftpp/numeric/numeric_cast_exception.cpp
  libftpp/optional/bad_optional_access.cpp
  libftpp/optional/nullopt_t.cpp
  libftpp/safe_bool/safe_bool_base.cpp
  libftpp/source_location/source_location.cpp
  libftpp/string/from_string_exception.cpp
  libftpp/string/to_string.cpp
  libftpp/string/trim.cpp
  libftpp/string/to_lower.cpp
  libftpp/string/to_upper.cpp
  libftpp/utility/demangle.cpp
)

set_target_properties(${LIB_NAME} PROPERTIES
  CXX_STANDARD 98
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_compile_options(${LIB_NAME}
  PRIVATE -Wall -Wextra -Werror -Wpedantic -Wshadow
)

target_include_directories(${LIB_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
