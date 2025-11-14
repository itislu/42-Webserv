/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParserTester.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lstefane <lstefane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:48:55 by lstefane          #+#    #+#             */
/*   Updated: 2025/11/14 12:40:28 by lstefane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TESTER

#include "config/Config.hpp"
#include "config/ConfigParser.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

/* TEST - 000 */
TEST(ConfigParserTest, ParsesSimpleServerBlock)
{
  const std::string configPath = std::string(ASSETS_PATH) + "000_simple.conf";
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

/* TEST - 001 */
TEST(ConfigParserTest, MutlipleServerBlock)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "001_multiple_servers.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 002 */
TEST(ConfigParserTest, WithLocations)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "002_with_locations.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 003 */
TEST(ConfigParserTest, ErrorPages)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "003_error_pages.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 004 */
TEST(ConfigParserTest, MultiPortServer)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "004_multiport_server.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 005 */
TEST(ConfigParserTest, MultiNameServer)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "005_multiname_server.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 006 */
TEST(ConfigParserTest, GlobalDirectiveDuplicate)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "006_global_directive_duplicate.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 007 */
TEST(ConfigParserTest, ServerDirectiveDuplicate)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "007_server_directive_duplicate.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 008 */
TEST(ConfigParserTest, LocationDirectiveDuplicate)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "008_location_directive_duplicate.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 009 */
TEST(ConfigParserTest, ServerWithMultipleLocations)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "009_server_multilocations.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 010 */
TEST(ConfigParserTest, MissingSemicolon)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "010_missing_semicolon.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 011 */
TEST(ConfigParserTest, MissingRBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "011_missing_rbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 012 */
TEST(ConfigParserTest, MissingLBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "012_missing_lbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 013 */
TEST(ConfigParserTest, MissingLocation)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "013_missing_location.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 014 */
TEST(ConfigParserTest, MissingListen)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "014_missing_listen.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 015 */
TEST(ConfigParserTest, MissingServerNames)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "015_missing_servernames.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 016 */
TEST(ConfigParserTest, MissingServerBlock)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "016_missing_server.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 017 */
TEST(ConfigParserTest, MissingLocationLBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "017_missing_location_lbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 018 */
TEST(ConfigParserTest, MissingLocationRBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "018_missing_location_rbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 019 */
TEST(ConfigParserTest, NoDirectiveBeforeSemicolon)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "019_missing_directive.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 020 */
TEST(ConfigParserTest, MissingValue)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "020_missing_value.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 021 */
TEST(ConfigParserTest, NoConfFile)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "021_no_conf_file.config";
  EXPECT_THROW(const ConfigParser parser(configPath.c_str()),
               std::invalid_argument);
}

/* TEST - 022 */
TEST(ConfigParserTest, NoConfFile02)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "022_no_conf_file.txt";
  EXPECT_THROW(const ConfigParser parser(configPath.c_str()),
               std::invalid_argument);
}

/* TEST - 023 */
TEST(ConfigParserTest, InvalidValue)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "023_invalid_value.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 024 */
TEST(ConfigParserTest, InvalidPort)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "024_invalid_port.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 025 */
TEST(ConfigParserTest, InvalidPort2)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "025_invalid_port2.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 026 */
TEST(ConfigParserTest, NegativePort)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "026_negative_port.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 027 */
TEST(ConfigParserTest, ZeroPort)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "027_zero_port.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 028 */
TEST(ConfigParserTest, BodySizeMultiplier_b)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "028_bodysize_multiplier_b.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 029 */
TEST(ConfigParserTest, BodySizeMultiplier_B)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "029_bodysize_multiplier_B.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 030 */
TEST(ConfigParserTest, BodySizeMultiplier_k)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "030_bodysize_multiplier_k.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 031 */
TEST(ConfigParserTest, BodySizeMultiplier_K)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "031_bodysize_multiplier_K.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 032 */
TEST(ConfigParserTest, BodySizeMultiplier_m)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "032_bodysize_multiplier_m.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 033 */
TEST(ConfigParserTest, BodySizeMultiplier_M)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "033_bodysize_multiplier_M.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 034 */
TEST(ConfigParserTest, BodySizeMultiplier_g)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "034_bodysize_multiplier_g.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 035 */
TEST(ConfigParserTest, BodySizeMultiplier_G)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "035_bodysize_multiplier_G.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 036 */
TEST(ConfigParserTest, BodySizeMultiplierInvalid)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "036_bodysize_multiplier_invalid.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 037 */
TEST(ConfigParserTest, InvalidErrorCode)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "037_invalid_errorcode.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 038 */
TEST(ConfigParserTest, InvalidErrorCode02)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "038_invalid_errorcode02.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 039 */
TEST(ConfigParserTest, InvalidErrorCode03)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "039_invalid_errorcode03.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 040 */
TEST(ConfigParserTest, OverwriteErrorCode)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "040_overwrite_errorcode.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 041 */
TEST(ConfigParserTest, OverwriteErrorCode02)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "041_overwrite_errorcode02.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 042 */
TEST(ConfigParserTest, TestAllValues)
{
  const std::string path =
    std::string(ASSETS_PATH) + "042_test_all_values.conf";
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
  EXPECT_TRUE(loc1.isAutoindex());
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

/* TEST - 043 */
TEST(ConfigParserTest, LargeValues)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "043_large_values.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 044 */
TEST(ConfigParserTest, MinimalConfig)
{
  const std::string configPath = std::string(ASSETS_PATH) + "044_minimal.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 045 */
TEST(ConfigParserTest, AdditionalLBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "045_additional_lbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 046 */
TEST(ConfigParserTest, AdditionalRBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "046_additional_rbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 047 */
TEST(ConfigParserTest, AdditionalSemicolon)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "047_additonal_semicolon.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 048 */
TEST(ConfigParserTest, EmptyLinesSemicolon)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "048_emptylines_semicolon.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 049 */
TEST(ConfigParserTest, CommentsInValue)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "049_comments_in_value.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 050 */
TEST(ConfigParserTest, EmptyFile)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "050_empty_file.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 051 */
TEST(ConfigParserTest, EmptyLines)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "051_empty_lines.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 052 */
TEST(ConfigParserTest, CommentsOnly)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "052_comments_only.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 053 */
TEST(ConfigParserTest, DuplicatedPort01)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "053_duplicated_port01.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 054 */
TEST(ConfigParserTest, DuplicatedPort02)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "054_duplicated_port02.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 055 */
TEST(ConfigParserTest, GlobalRoot)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "055_global_root.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 056 */
TEST(ConfigParserTest, DoubleBracers)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "056_double_bracers.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 057 */
TEST(ConfigParserTest, EmptyServerBlock)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "057_empty_server.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 058 */
TEST(ConfigParserTest, EmptyLocationBlock)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "058_empty_location.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 059 */
TEST(ConfigParserTest, OverlappingPorts)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "059_overlapping_ports.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 060 */
TEST(ConfigParserTest, OverlappingHostnames)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "060_overlapping_hostnames.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 061 */
TEST(ConfigParserTest, DuplicateLocations)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "061_duplicate_locations.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 062 */
TEST(ConfigParserTest, SemicolonBeforeDirective)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "062_semicolon_before.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 063 */
TEST(ConfigParserTest, NewlinesInDirectives)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "063_newlines_in_directives.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 064 */
TEST(ConfigParserTest, NegativeBodySize)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "064_negative_body_size.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 065 */
TEST(ConfigParserTest, NotAllowedMethod)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "065_not_allowed_method.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 066 */
TEST(ConfigParserTest, NotAllowedMethod02)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "066_not_allowed_method02.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 067 */
TEST(ConfigParserTest, InvalidErrorCode04)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "067_invalid_errorcode04.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 068 */
TEST(ConfigParserTest, InvalidPorts03)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "068_invalid_ports03.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 069 */
TEST(ConfigParserTest, DuplicatedMethods)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "069_duplicated_methods.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 070 */
TEST(ConfigParserTest, InvalidDirective00)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "070_invalid_directive00.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 071 */
TEST(ConfigParserTest, InvalidDirective01)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "071_invalid_directive01.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 072 */
TEST(ConfigParserTest, InvalidDirective02)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "072_invalid_directive02.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 073 */
TEST(ConfigParserTest, ErrorCodes00)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "073_errorcodes00.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 074 */
TEST(ConfigParserTest, ErrorCodes01)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "074_errorcodes01.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 075 */
TEST(ConfigParserTest, ErrorCodes02)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "075_errorcodes02.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

/* TEST - 076 */
TEST(ConfigParserTest, ErrorCodes03)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "076_errorcodes03.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

/* TEST - 077 */
TEST(ConfigParserTest, ErrorCodes04)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "077_errorcodes04.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
