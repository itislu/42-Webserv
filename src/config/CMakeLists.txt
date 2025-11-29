# **************************************************************************** #
# CONFIG MODULE
# **************************************************************************** #

set(LIB_NAME config)

add_library(${LIB_NAME} STATIC
   Config.cpp
   ConfigBuilder.cpp
   ConfigParser.cpp
   Converters.cpp
   DirectiveHandler.cpp
   Lexer.cpp
   LocationBuilder.cpp
   LocationConfig.cpp
   ParsedConfig.cpp
   ParsedLocation.cpp
   ParsedServer.cpp
   ServerBuilder.cpp
   ServerConfig.cpp
   Token.cpp
 )

target_link_libraries(${LIB_NAME} 
  PRIVATE
  ftpp
)
