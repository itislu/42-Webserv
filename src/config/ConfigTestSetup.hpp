// ConfigTestSetup.hpp
#pragma once

#include "Config.hpp"
#include "LocationConfig.hpp"
#include "ServerConfig.hpp"
#include <cstddef>
#include <iostream>
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
                                     const std::string& root = "./www")
{
  ServerConfig server(config);

  // Add ports
  for (std::size_t i = 0; i < ports.size(); ++i) {
    server.addPort(ports[i]);
  }

  server.setRoot(root);

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

  config.setDefaultMaxBodySize(MAXBODYSIZE);
  config.setDefaultTimeout(TIMEOUT); // 60 seconds

  // Server 1: ports
  std::vector<int> ports1;
  ports1.push_back(8080);
  ports1.push_back(8081);
  ServerConfig server1 = createTestServer(config, ports1, "./www/server1");
  config.addServer(server1);

  // Server 2: single port
  std::vector<int> ports2;
  ports2.push_back(9090);
  ServerConfig server2 = createTestServer(config, ports2, "./www/server2");
  config.addServer(server2);

  return config;
}

Config testConfig()
{
  const Config config = TestConfigSetup::createTestConfig();

  const std::vector<ServerConfig>& servers = config.getServers();
  for (std::vector<ServerConfig>::const_iterator serverIt = servers.begin();
       serverIt != servers.end();
       ++serverIt) {
    std::cout << "Server should listen on ports: ";

    const std::vector<int>& ports = serverIt->getPorts();
    for (std::vector<int>::const_iterator portIt = ports.begin();
         portIt != ports.end();
         ++portIt) {
      std::cout << *portIt << " ";
    }
    std::cout << "\n";

    const std::vector<LocationConfig>& locations = serverIt->getLocations();
    for (std::vector<LocationConfig>::const_iterator locIt = locations.begin();
         locIt != locations.end();
         ++locIt) {
      std::cout << "  Location: " << locIt->getPath()
                << ", root: " << locIt->getRoot()
                << ", autoindex: " << (locIt->isAutoindex() ? "on" : "off")
                << "\n";
    }
  }

  return config;
}

// NOLINTEND

} // namespace
