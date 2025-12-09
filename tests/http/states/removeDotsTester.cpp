#include "http/states/validateRequest/ValidateRequest.hpp"
#include <gtest/gtest.h>

// NOLINTBEGIN

#include <string>

// 1. Define a struct to hold your test data
struct PathTestParams
{
  std::string testName;
  std::string input;
  std::string expected;
};

// 2. Create a Test Fixture class
class RemoveDotSegmentsTest : public ::testing::TestWithParam<PathTestParams>
{
  // You can add setup/teardown here if needed
};

// 3. Define the actual test logic ONCE
TEST_P(RemoveDotSegmentsTest, NormalizesCorrectly)
{
  PathTestParams params = GetParam();

  // Replace this with your actual class/function call
  std::string actual = ValidateRequest::removeDotSegments(params.input);

  EXPECT_EQ(actual, params.expected)
    << "Failed on test case: " << params.testName;
}

// 4. Instantiate the test suite with all your cases
INSTANTIATE_TEST_SUITE_P(
  Test,
  RemoveDotSegmentsTest,
  ::testing::Values(

    // Basic
    PathTestParams{ "Basic", "/a/b/c", "/a/b/c" },
    PathTestParams{ "KeepTrailingSlash", "/file/", "/file/" },

    // Root / Empty
    PathTestParams{ "Empty", "", "" },
    PathTestParams{ "Root", "/", "/" },

    // RFC Rule A (Prefixes)
    PathTestParams{ "PrefixCurrent", "./a", "a" },
    PathTestParams{ "PrefixParent", "../a", "a" },

    // RFC Rule B (Internal Current)
    PathTestParams{ "InternalCurrent", "/a/./b", "/a/b" },
    PathTestParams{ "InternalCurrentRoot", "/./a", "/a" },

    // RFC Rule C (Internal Parent)
    PathTestParams{ "InternalParent", "/a/b/../c", "/a/c" },
    PathTestParams{ "RootParent", "/../a", "/a" },

    // Trailing Handling
    PathTestParams{ "TrailingSlash", "/a/b/", "/a/b/" },
    PathTestParams{ "TrailingDot", "/a/b/.", "/a/b/" },
    PathTestParams{ "TrailingDoubleDot", "/a/b/..", "/a/" },

    // Weird / False Positives
    PathTestParams{ "ThreeDots", "/a/...", "/a/..." },
    PathTestParams{ "HiddenFile", "/.gitignore", "/.gitignore" },
    PathTestParams{ "DotsInName", "/a/..image.jpg", "/a/..image.jpg" },

    // Complex
    PathTestParams{ "ComplexRFC", "/a/b/c/./../../g", "/a/g" },
    PathTestParams{ "RootBlast", "../../a", "a" }),

  // Optional: Print the test name in the output for clearer debugging
  [](const testing::TestParamInfo<PathTestParams>& info) {
    return info.param.testName;
  });

// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
