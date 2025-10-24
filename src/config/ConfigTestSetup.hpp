// ConfigTestSetup.hpp
#pragma once

#include "Config.hpp"
#include "LocationConfig.hpp"
#include "ServerConfig.hpp"
#include <cstddef>
#include <string>
#include <vector>

#ifndef MAXBODYSIZE
#define MAXBODYSIZE 1024
#endif

#ifndef TIMEOUT
#define TIMEOUT 60
#endif

namespace TestConfigSetup {

// NOLINTBEGIN
static LocationConfig createTestLocation(
  const ServerConfig& server,
  const std::string& path,
  const std::string& root,
  bool autoindex,
  const std::vector<std::string>& methods)
{
  LocationConfig loc(server);
  loc.setPath(path);
  loc.setRoot(root);
  loc.setAutoindex(autoindex);
  for (std::size_t idx = 0; idx < methods.size(); ++idx) {
    loc.addAllowedMethods(methods[idx]);
  }
  loc.setIndex("index.html");
  return loc;
}

// --- Create a test ServerConfig ---
static ServerConfig createTestServer(const Config& config,
                                     const std::vector<int>& ports,
                                     const std::vector<std::string>& hosts,
                                     const std::size_t timeout,
                                     const std::string& root = "./www")
{
  ServerConfig server(config);

  // Add ports
  for (std::size_t i = 0; i < ports.size(); ++i) {
    server.addPort(ports[i]);
  }
  // Add hosts
  for (std::size_t i = 0; i < hosts.size(); ++i) {
    server.addHostName(hosts[i]);
  }

  server.setRoot(root);

  server.setTimeout(timeout);

  std::vector<std::string> methods;
  methods.push_back("GET");
  methods.push_back("POST");
  methods.push_back("DELETE");

  // Create default location /
  server.addLocation(createTestLocation(server, "/", root, false, methods));

  // Add another example location
  server.addLocation(
    createTestLocation(server, "/tours", root + "/tours", true, methods));

  return server;
}

// --- Create a global Config with 2 servers ---
Config createTestConfig()
{
  Config config("file");

  config.setMaxBodySize(MAXBODYSIZE);
  config.setTimeout(TIMEOUT); // 60 seconds

  // Server 1: ports
  std::vector<int> ports1;
  ports1.push_back(8080);
  ports1.push_back(8081);
  std::vector<std::string> hosts1;
  hosts1.push_back("test.com");
  ServerConfig server1 =
    createTestServer(config, ports1, hosts1, 60, "./www/server1");
  config.addServer(server1);

  // Server 2: single port
  std::vector<int> ports2;
  ports2.push_back(8080);
  ports2.push_back(9090);
  std::vector<std::string> hosts2;
  hosts2.push_back("example.com");
  hosts2.push_back("localhost");
  ServerConfig server2 =
    createTestServer(config, ports2, hosts2, 60, "./www/server2");
  config.addServer(server2);
  config.setDefaultTimeout();

  return config;
}

// NOLINTEND

} // namespace
