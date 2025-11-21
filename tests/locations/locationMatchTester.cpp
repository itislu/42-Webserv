#include "config/Config.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include "libftpp/utility.hpp"
#include <cstddef>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

using config::Config;
using config::LocationConfig;
using config::ServerConfig;

TEST(LocationMatchTest, SimplePrefixMatch)
{
  const Config config;
  ServerConfig server(config);

  LocationConfig img(server);
  img.setPath("/image");
  img.setRoot("/data/image");
  server.addLocation(img);

  const LocationConfig* const loc =
    server.getBestMatchLocation("/image/logo.png");
  ASSERT_TRUE(loc != NULL);
  EXPECT_EQ(loc->getPath(), "/image");
}

TEST(LocationMatchTest, LongestPrefixMatch)
{
  const Config config;
  ServerConfig server(config);

  LocationConfig img(server);
  img.setPath("/image");
  server.addLocation(img);

  LocationConfig imgCat(server);
  imgCat.setPath("/image/cat");
  server.addLocation(imgCat);

  const LocationConfig* const loc =
    server.getBestMatchLocation("/image/cat/kitty.png");
  ASSERT_TRUE(loc != NULL);
  EXPECT_EQ(loc->getPath(), "/image/cat");
}

TEST(LocationMatchTest, NoMatch)
{
  const Config config;
  ServerConfig server(config);

  LocationConfig test(server);
  test.setPath("/test");
  server.addLocation(test);

  const LocationConfig* const loc =
    server.getBestMatchLocation("/unknown/path");
  EXPECT_TRUE(loc == NULL);
}

TEST(LocationMatchTest, ExactMatch)
{
  const Config config;
  ServerConfig server(config);

  LocationConfig api(server);
  api.setPath("/api");
  server.addLocation(api);

  LocationConfig test(server);
  test.setPath("/api/test");
  server.addLocation(test);

  const LocationConfig* const loc = server.getBestMatchLocation("/api");
  ASSERT_TRUE(loc != NULL);
  EXPECT_EQ(loc->getPath(), "/api");
}

TEST(LocationMatchTest, DirectoryBoundary)
{
  const Config config;
  ServerConfig server(config);

  LocationConfig app(server);
  app.setPath("/app");
  server.addLocation(app);

  LocationConfig application(server);
  application.setPath("/application");
  server.addLocation(application);

  const LocationConfig* const loc1 = server.getBestMatchLocation("/app/data");
  ASSERT_TRUE(loc1 != NULL);
  EXPECT_EQ(loc1->getPath(), "/app");

  const LocationConfig* const loc2 =
    server.getBestMatchLocation("/application/data");
  ASSERT_TRUE(loc2 != NULL);
  EXPECT_EQ(loc2->getPath(), "/application");
}

TEST(LocationMatchTest, TrailingSlashEquality)
{
  const Config config;
  ServerConfig server(config);

  LocationConfig img(server);
  img.setPath("/image");
  server.addLocation(img);

  const LocationConfig* const loc = server.getBestMatchLocation("/image/");
  ASSERT_TRUE(loc != NULL);
  EXPECT_EQ(loc->getPath(), "/image");
}

TEST(LocationMatchTest, RootLocationFallback)
{
  const Config config;
  ServerConfig server(config);

  LocationConfig root(server);
  root.setPath("/");
  server.addLocation(root);

  LocationConfig api(server);
  api.setPath("/api");
  server.addLocation(api);

  const LocationConfig* const loc1 = server.getBestMatchLocation("/api/test");
  ASSERT_TRUE(loc1 != NULL);
  EXPECT_EQ(loc1->getPath(), "/api");

  const LocationConfig* const loc2 = server.getBestMatchLocation("/something");
  ASSERT_TRUE(loc2 != NULL);
  EXPECT_EQ(loc2->getPath(), "/");
}

TEST(LocationMatchTest, DeepNestedMatch)
{
  const Config config;

  ServerConfig server(config);

  LocationConfig a(server);
  a.setPath("/a");
  server.addLocation(a);
  LocationConfig ab(server);
  ab.setPath("/a/b");
  server.addLocation(ab);
  LocationConfig abc(server);
  abc.setPath("/a/b/c");
  server.addLocation(abc);

  const LocationConfig* loc = server.getBestMatchLocation("/a/b/c/d/e");
  ASSERT_TRUE(loc != NULL);
  EXPECT_EQ(loc->getPath(), "/a/b/c");
}

TEST(LocationMatchTest, SameLengthPrefixChoosing)
{
  const Config config;

  ServerConfig server(config);

  LocationConfig loc1(server);
  loc1.setPath("/abc");
  server.addLocation(loc1);
  LocationConfig loc2(server);
  loc2.setPath("/abd");
  server.addLocation(loc2);

  const LocationConfig* loc = server.getBestMatchLocation("/abd/test");
  ASSERT_TRUE(loc != NULL);
  EXPECT_EQ(loc->getPath(), "/abd");
}

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
