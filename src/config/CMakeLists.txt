# **************************************************************************** #
# CONFIG MODULE
# **************************************************************************** #

set(LIB_NAME config)

add_library(${LIB_NAME} STATIC
   Config.cpp
   LocationConfig.cpp
   ServerConfig.cpp
   parser/ConfigBuilder.cpp
   parser/ConfigParser.cpp
   parser/Converters.cpp
   parser/DirectiveHandler.cpp
   parser/Lexer.cpp
   parser/LocationBuilder.cpp
   parser/ParsedConfig.cpp
   parser/ParsedLocation.cpp
   parser/ParsedServer.cpp
   parser/ServerBuilder.cpp
   parser/Token.cpp
 )

target_link_libraries(${LIB_NAME} 
  PRIVATE
  ftpp
  utils
)
