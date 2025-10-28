# **************************************************************************** #
# CONFIG MODULE
# **************************************************************************** #

set(LIB_NAME config)

add_library(${LIB_NAME} STATIC
   Config.cpp
   ConfigParser.cpp
   FileUtils.cpp
   LocationConfig.cpp
   ServerConfig.cpp
 )

target_link_libraries(${LIB_NAME} 
  PRIVATE
  ftpp
)
