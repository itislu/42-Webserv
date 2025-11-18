#include "config/Config.hpp"
#include "config/ConfigParser.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

using config::Config;
using config::ConfigParser;
using config::LocationConfig;
using config::ServerConfig;

TEST(ConfigParserTest, ParsesSimpleServerBlock)
{
  const std::string configPath = std::string(ASSETS_PATH) + "simple.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());

  EXPECT_EQ(config.getRoot(), "/var/www/");
  EXPECT_EQ(config.getMaxBodySize(), 1024);
  EXPECT_EQ(config.getTimeout(), 60);
  EXPECT_EQ(config.getServers().size(), 1);
  EXPECT_EQ(config.getServers()[0].getPorts()[0], 8080);
  EXPECT_EQ(config.getServers()[0].getHostnames()[0], "serv1");
}

TEST(ConfigParserTest, MultipleServerBlockk)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "multiple_servers.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, WithLocations)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "with_locations.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, ErrorPages)
{
  const std::string configPath = std::string(ASSETS_PATH) + "error_pages.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, MultiPortServer)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "multiport_server.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, MultiNameServer)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "multiname_server.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, GlobalDirectiveDuplicate)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "global_directive_duplicate.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, ServerDirectiveDuplicate)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "server_directive_duplicate.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, LocationDirectiveDuplicate)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "location_directive_duplicate.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, ServerWithMultipleLocations)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "server_multilocations.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, MissingSemicolon)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_semicolon.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, MissingRBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_rbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, MissingLBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_lbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, MissingLocation)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_location.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, MissingListen)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_listen.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, MissingServerNames)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_servernames.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, MissingServerBlock)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_server.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, MissingLocationLBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_location_lbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, MissingLocationRBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_location_rbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, NoDirectiveBeforeSemicolon)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_directive.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, MissingValue)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_value.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, NoConfFile)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "no_conf_file.config";
  EXPECT_THROW(const ConfigParser parser(configPath.c_str()),
               std::invalid_argument);
}

TEST(ConfigParserTest, NoConfFile02)
{
  const std::string configPath = std::string(ASSETS_PATH) + "no_conf_file.txt";
  EXPECT_THROW(const ConfigParser parser(configPath.c_str()),
               std::invalid_argument);
}

TEST(ConfigParserTest, InvalidValue)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_value.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, InvalidPort)
{
  const std::string configPath = std::string(ASSETS_PATH) + "invalid_port.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, InvalidPort2)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_port2.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, NegativePort)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "negative_port.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, ZeroPort)
{
  const std::string configPath = std::string(ASSETS_PATH) + "zero_port.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, BodySizeMultiplier_b)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "bodysize_multiplier_b.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, BodySizeMultiplier_B)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "bodysize_multiplier_B.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, BodySizeMultiplier_k)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "bodysize_multiplier_k.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, BodySizeMultiplier_K)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "bodysize_multiplier_K.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, BodySizeMultiplier_m)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "bodysize_multiplier_m.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, BodySizeMultiplier_M)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "bodysize_multiplier_M.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, BodySizeMultiplier_g)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "bodysize_multiplier_g.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, BodySizeMultiplier_G)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "bodysize_multiplier_G.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, BodySizeMultiplierInvalid)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "bodysize_multiplier_invalid.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, InvalidErrorCode)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_errorcode.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, InvalidErrorCode02)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_errorcode02.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, InvalidErrorCode03)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_errorcode03.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, OverwriteErrorCode)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overwrite_errorcode.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, OverwriteErrorCode02)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overwrite_errorcode02.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, TestAllValues)
{
  const std::string path = std::string(ASSETS_PATH) + "test_all_values.conf";
  ConfigParser parser(path.c_str());
  Config config;

  EXPECT_NO_THROW(config = parser.parseConfig());

  // Global
  EXPECT_EQ(config.getRoot(), "/var/www/global/");
  EXPECT_EQ(config.getMaxBodySize(), 8192);
  EXPECT_EQ(config.getTimeout(), 120);
  EXPECT_EQ(config.getErrorPages().at(400), "/errors/400.html");
  EXPECT_EQ(config.getErrorPages().at(404), "/errors/404.html");

  // Servers
  EXPECT_EQ(config.getServers().size(), 2);

  const ServerConfig& srv1 = config.getServers()[0];
  EXPECT_EQ(srv1.getPorts().size(), 2);
  EXPECT_EQ(srv1.getPorts()[0], 8080);
  EXPECT_EQ(srv1.getHostnames()[0], "example.com");
  EXPECT_EQ(srv1.getRoot(), "/var/www/example/");
  EXPECT_EQ(srv1.getIndex(), "index.html");
  EXPECT_EQ(srv1.getMaxBodySize(), 4096);
  EXPECT_EQ(srv1.getErrorPages().at(403), "/errors/403.html");
  EXPECT_TRUE(srv1.getAllowedMethods().count("GET"));
  EXPECT_TRUE(srv1.getAllowedMethods().count("POST"));
  EXPECT_TRUE(srv1.getAllowedMethods().count("DELETE"));

  const LocationConfig& loc1 = srv1.getLocations()[0];
  EXPECT_EQ(loc1.getPath(), "/");
  EXPECT_EQ(loc1.getRoot(), "/var/www/example/public/");
  EXPECT_TRUE(loc1.isAutoIndex());
  EXPECT_EQ(loc1.getIndex(), "index.html");
  EXPECT_EQ(loc1.getMaxBodySize(), 2048);
  EXPECT_EQ(loc1.getErrorPages().at(404), "/errors/custom_404.html");

  const LocationConfig& loc3 = srv1.getLocations()[2]; // /cgi-bin/
  EXPECT_TRUE(loc3.isCgi());
  EXPECT_EQ(loc3.getCgiPass(), "/usr/bin/python3");
  EXPECT_EQ(loc3.getCgiExtension(), ".py");

  const LocationConfig& loc4 = srv1.getLocations()[3]; // /old/
  EXPECT_TRUE(loc4.isRedirect());
  EXPECT_EQ(loc4.getRedirectCode(), 301);
  EXPECT_EQ(loc4.getRedirection(), "http://example.com/new/");

  const ServerConfig& srv2 = config.getServers()[1];
  EXPECT_EQ(srv2.getPorts()[0], 9090);
  EXPECT_EQ(srv2.getHostnames()[0], "api.example.com");
  EXPECT_EQ(srv2.getRoot(), "/var/www/api/");
  EXPECT_EQ(srv2.getMaxBodySize(), 16384);
  EXPECT_EQ(srv2.getTimeout(), 60);
}

TEST(ConfigParserTest, LargeValues)
{
  const std::string configPath = std::string(ASSETS_PATH) + "large_values.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, MinimalConfig)
{
  const std::string configPath = std::string(ASSETS_PATH) + "minimal.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, AdditionalLBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "additional_lbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, AdditionalRBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "additional_rbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, AdditionalSemicolon)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "additional_semicolon.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, EmptyLinesSemicolon)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "emptylines_semicolon.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, CommentsInValue)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "comments_in_value.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, EmptyFile)
{
  const std::string configPath = std::string(ASSETS_PATH) + "empty_file.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, EmptyLines)
{
  const std::string configPath = std::string(ASSETS_PATH) + "empty_lines.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, CommentsOnly)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "comments_only.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, DuplicatedPort01)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "duplicated_port01.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, DuplicatedPort02)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "duplicated_port02.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, GlobalRoot)
{
  const std::string configPath = std::string(ASSETS_PATH) + "global_root.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, DoubleBraces)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "double_braces.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, EmptyServerBlock)
{
  const std::string configPath = std::string(ASSETS_PATH) + "empty_server.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, EmptyLocationBlock)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "empty_location.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, OverlappingPorts)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overlapping_ports.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, OverlappingHostnames)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overlapping_hostnames.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, DuplicateLocations)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "duplicate_locations.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, SemicolonBeforeDirective)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "semicolon_before.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, NewlinesInDirectives)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "newlines_in_directives.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, NegativeBodySize)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "negative_body_size.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, NotAllowedMethod)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "not_allowed_method.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, NotAllowedMethod02)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "not_allowed_method02.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, InvalidErrorCode04)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_errorcode04.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, InvalidPorts03)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_ports03.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, DuplicatedMethods)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "duplicated_methods.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, InvalidDirective00)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_directive00.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, InvalidDirective01)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_directive01.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, InvalidDirective02)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_directive02.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, ErrorCodes00)
{
  const std::string configPath = std::string(ASSETS_PATH) + "errorcodes00.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, ErrorCodes01)
{
  const std::string configPath = std::string(ASSETS_PATH) + "errorcodes01.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, ErrorCodes02)
{
  const std::string configPath = std::string(ASSETS_PATH) + "errorcodes02.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, ErrorCodes03)
{
  const std::string configPath = std::string(ASSETS_PATH) + "errorcodes03.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ConfigParserTest, ErrorCodes04)
{
  const std::string configPath = std::string(ASSETS_PATH) + "errorcodes04.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, NoValue)
{
  const std::string configPath = std::string(ASSETS_PATH) + "no_value.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, NegativeValue)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "negative_value.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, ErrorCode05)
{
  const std::string configPath = std::string(ASSETS_PATH) + "errorcodes05.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, NegativeZero)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "negative_zero.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, OverflowBodySize)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_bodysize.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, OverflowBodySizeMultiplier_B)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_bodysize_multiplier_B.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, OverflowBodySizeMultiplier_K)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_bodysize_multiplier_K.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::overflow_error);
}

TEST(ConfigParserTest, OverflowBodySizeMultiplier_M)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_bodysize_multiplier_M.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::overflow_error);
}

TEST(ConfigParserTest, OverflowBodySizeMultiplier_G)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_bodysize_multiplier_G.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::overflow_error);
}

TEST(ConfigParserTest, OverflowServerBodySize)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_server_bodysize.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, OverflowLocationBodySize)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_location_bodysize.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, OverflowTimeout)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_timeout.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, OverflowServerTimeout)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_server_timeout.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, OverflowErrorCode)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_errorcode.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, OverflowRedirectCode)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_redirect_code.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, OverflowPort)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_port.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, OverflowIntMax)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_int_max.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, OverflowIntMin)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_int_min.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, OverflowLongMax)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_long_max.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, OverflowLongMin)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_long_min.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(ConfigParserTest, OverflowSizeTMax)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow_sizet_max.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
