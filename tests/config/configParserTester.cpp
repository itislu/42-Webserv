#include "config/Config.hpp"
#include "config/ConfigParser.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <vector>

using config::Config;
using config::ConfigParser;
using config::LocationConfig;
using config::ServerConfig;

// ============================================================================
// Duplicates Tests
// ============================================================================

TEST(DuplicatesTester, DupDirectiveGlobal)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "duplicates/dup_directive_global.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(DuplicatesTester, DupDirectiveLocation)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "duplicates/dup_directive_location.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(DuplicatesTester, DupDirectiveServer)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "duplicates/dup_directive_server.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(DuplicatesTester, DupLocations)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "duplicates/dup_locations.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(DuplicatesTester, DupPortDifferentLine)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "duplicates/dup_port_different_line.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(DuplicatesTester, DupPortSameLine)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "duplicates/dup_port_same_line.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

// ============================================================================
// Empty Config Tests
// ============================================================================

TEST(EmptyConfigTester, CommentsOnly)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "empty/comments_only.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(EmptyConfigTester, EmptyFile)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "empty/empty_file.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(EmptyConfigTester, EmptyLines)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "empty/empty_lines.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(EmptyConfigTester, EmptyServer)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "empty/empty_server.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

// ============================================================================
// Invalid Files Tests
// ============================================================================

TEST(InvalidFilesTester, InvalidExtensionConfig)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_files/invalid_extension.config";
  EXPECT_THROW(const ConfigParser parser(configPath.c_str()),
               std::invalid_argument);
}

TEST(InvalidFilesTester, InvalidExtensionTxt)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_files/invalid_extension.txt";
  EXPECT_THROW(const ConfigParser parser(configPath.c_str()),
               std::invalid_argument);
}

// ============================================================================
// Invalid Values Tests
// ============================================================================

TEST(InvalidValuesTester, ErrorCodeNonNumber)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/error_code_non_number.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, ErrorCodeTooHigh)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/error_code_too_high.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, ErrorCodeTooLow)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/error_code_too_low.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, ErrorCodeTrailingText)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/error_code_trailing_text.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, ErrorPageExtraCodeAfter)
{
  const std::string configPath =
    std::string(ASSETS_PATH) +
    "invalid_values/error_page_extra_code_after.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, ErrorPageMultiCodesPaths)
{
  const std::string configPath =
    std::string(ASSETS_PATH) +
    "invalid_values/error_page_multi_codes_paths.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, ErrorPageNoCode)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/error_page_no_code.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, ErrorPageNoPath)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/error_page_no_path.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, ErrorPagePathBeforeCode)
{
  const std::string configPath =
    std::string(ASSETS_PATH) +
    "invalid_values/error_page_path_before_code.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, InvalidBodysizeMultiplier)
{
  const std::string configPath =
    std::string(ASSETS_PATH) +
    "invalid_values/invalid_bodysize_multiplier.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, InvalidDirectiveGlobal)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/invalid_directive_global.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, InvalidDirectiveLocation)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/invalid_directive_location.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, InvalidDirectiveServer)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/invalid_directive_server.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, InvalidMethodLocation)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/invalid_method_location.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, InvalidMethodServer)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/invalid_method_server.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, NegativeBodySize)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/negative_bodysize.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, NegativePort)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/negative_port.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, NegativeZeroTimeout)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/negative_zero_timeout.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, PortNonNumber)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/port_non_number.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, PortNonNumberIncluded)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/port_non_number_included.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, PortTooHigh)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/port_too_high.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, TimeoutTrailingText)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/timeout_trailing_text.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(InvalidValuesTester, ZeroPort)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "invalid_values/zero_port.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

// ============================================================================
// Missing Required Tests
// ============================================================================

TEST(MissingRequiredTester, MissingDirective)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_required/missing_directive.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(MissingRequiredTester, MissingListen)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_required/missing_listen.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(MissingRequiredTester, MissingLocation)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_required/missing_location.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(MissingRequiredTester, MissingServer)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_required/missing_server.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(MissingRequiredTester, MissingServernames)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_required/missing_servernames.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(MissingRequiredTester, MissingTimeoutValue)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "missing_required/missing_timeout_value.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

// ============================================================================
// Overflow Tests
// ============================================================================

TEST(OverflowTester, OverflowBodysize)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_bodysize.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(OverflowTester, OverflowBodysizeMultiplier_B)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_bodysize_multiplier_B.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(OverflowTester, OverflowBodysizeMultiplier_G)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_bodysize_multiplier_G.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::overflow_error);
}

TEST(OverflowTester, OverflowBodysizeMultiplier_K)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_bodysize_multiplier_K.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::overflow_error);
}

TEST(OverflowTester, OverflowBodysizeMultiplier_M)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_bodysize_multiplier_M.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::overflow_error);
}

TEST(OverflowTester, OverflowErrorCode)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_error_code.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(OverflowTester, OverflowIntMax)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_int_max.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(OverflowTester, OverflowIntMin)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_int_min.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(OverflowTester, OverflowLocationBodysize)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_location_bodysize.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(OverflowTester, OverflowLongMax)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_long_max.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(OverflowTester, OverflowLongMin)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_long_min.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(OverflowTester, OverflowPort)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_port.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(OverflowTester, OverflowRedirectCode)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_redirect_code.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(OverflowTester, OverflowServerBodysize)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_server_bodysize.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(OverflowTester, OverflowServerTimeout)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_server_timeout.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(OverflowTester, OverflowSizeTMax)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_sizet_max.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(OverflowTester, OverflowTimeout)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "overflow/overflow_timeout.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

// ============================================================================
// Syntax Errors Tests
// ============================================================================

TEST(SyntaxErrorsTester, AdditionalLBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "syntax_errors/additional_lbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(SyntaxErrorsTester, AdditionalRBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "syntax_errors/additional_rbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(SyntaxErrorsTester, AdditionalSemicolon)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "syntax_errors/additional_semicolon.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(SyntaxErrorsTester, DoubleBraces)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "syntax_errors/double_braces.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(SyntaxErrorsTester, MissingLBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "syntax_errors/missing_lbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(SyntaxErrorsTester, MissingLocationLBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "syntax_errors/missing_location_lbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(SyntaxErrorsTester, MissingLocationRBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "syntax_errors/missing_location_rbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(SyntaxErrorsTester, MissingRBrace)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "syntax_errors/missing_rbrace.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(SyntaxErrorsTester, MissingSemicolon)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "syntax_errors/missing_semicolon.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

TEST(SyntaxErrorsTester, SemicolonBeforeDirective)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "syntax_errors/semicolon_before_directive.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_THROW(config = parser.parseConfig(), std::invalid_argument);
}

// ============================================================================
// Valid Config Tests
// ============================================================================

TEST(ValidConfigTester, BodySizeMultipliers)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/bodysize_multipliers.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  ASSERT_NO_THROW(config = parser.parseConfig());

  const std::vector<ServerConfig>& servers = config.getServers();
  ASSERT_EQ(servers.size(), 8);

  // b (bytes)
  EXPECT_EQ(servers[0].getMaxBodySize(), 1UL);
  // B (bytes)
  EXPECT_EQ(servers[1].getMaxBodySize(), 2UL);
  // k (kilobytes)
  EXPECT_EQ(servers[2].getMaxBodySize(), 3UL * 1024);
  // K (kilobytes)
  EXPECT_EQ(servers[3].getMaxBodySize(), 4UL * 1024);
  // m (megabytes)
  EXPECT_EQ(servers[4].getMaxBodySize(), 5UL * 1024 * 1024);
  // M (megabytes)
  EXPECT_EQ(servers[5].getMaxBodySize(), 6UL * 1024 * 1024);
  // g (gigabytes)
  EXPECT_EQ(servers[6].getMaxBodySize(), 7UL * 1024 * 1024 * 1024);
  // G (gigabytes)
  EXPECT_EQ(servers[7].getMaxBodySize(), 8UL * 1024 * 1024 * 1024);
}

TEST(ValidConfigTester, CommentsInValue)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/comments_in_value.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, DuplicateMethods)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/duplicate_methods.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, EmptyLinesSemicolon)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/empty_lines_semicolon.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, EmptyLocation)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/empty_location.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, ErrorPages)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/error_pages.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, GlobalRoot)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/global_root.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, LargeValues)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/large_values.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, Minimal)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/minimal.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, MultinameServer)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/multiname_server.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, MultipleLocations)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/multiple_locations.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, MultipleServers)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/multiple_servers.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, MultiportServer)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/multiport_server.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, NewlinesInDirectives)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/newlines_in_directives.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, OverlappingHostnames)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/overlapping_hostnames.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, OverlappingPorts)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/overlapping_ports.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, OverwriteErrorcodeLocation)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/overwrite_errorcode_location.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, OverwriteErrorcodeServer)
{
  const std::string configPath =
    std::string(ASSETS_PATH) + "valid/overwrite_errorcode_server.conf";
  ConfigParser parser(configPath.c_str());
  Config config;
  EXPECT_NO_THROW(config = parser.parseConfig());
}

TEST(ValidConfigTester, Simple)
{
  const std::string configPath = std::string(ASSETS_PATH) + "valid/simple.conf";
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

TEST(ValidConfigTester, TestAllValues)
{
  const std::string path =
    std::string(ASSETS_PATH) + "valid/test_all_values.conf";
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

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
