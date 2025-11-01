#include "config/Config.hpp"
#include "config/ConfigTestSetup.hpp"
#include "config/ServerConfig.hpp"
#include "http/http.hpp"
#include "libftpp/memory.hpp"
#include "libftpp/utility.hpp"
#include "server/ServerManager.hpp"
#include "utils/abnfRules/RangeRule.hpp"
#include "utils/abnfRules/RepetitionRule.hpp"
#include "utils/abnfRules/SequenceRule.hpp"
#include <exception>
#include <iostream>
#include <string>

// ft::unique_ptr<SequenceRule> schemeRule()
// {
//   // RangeRule* const range = new RangeRule(http::isSchemeChar);
//   ft::unique_ptr<RangeRule> range =
//     ft::make_unique<RangeRule>(http::isSchemeChar);
//   ft::unique_ptr<RepetitionRule> rep =
//     ft::make_unique<RepetitionRule>(ft::move(range));

//   ft::unique_ptr<SequenceRule> seq = ft::make_unique<SequenceRule>();
//   // seq->addRule(ft::unique_ptr<RangeRule>(new RangeRule(::isalpha)));
//   // seq->addRule(ft::make_unique<RangeRule>(::isalpha));
//   // seq->addRule(ft::move(rep));

//   seq->setDebugTag("schemeRule");
//   return ft::move(seq);
// }

void take(ft::rvalue<std::string>& str)
{
  (void)str;
}

ft::unique_ptr<SequenceRule> schemeRule()
{
  // RangeRule* const range = new RangeRule(http::isSchemeChar);
  ft::unique_ptr<RangeRule> range =
    ft::make_unique<RangeRule>(http::isSchemeChar);
  ft::unique_ptr<RepetitionRule> rep =
    ft::make_unique<RepetitionRule>(ft::move(range));

  ft::unique_ptr<SequenceRule> seq = ft::make_unique<SequenceRule>();
  // seq->addRule(ft::unique_ptr<RangeRule>(new RangeRule(::isalpha)));
  // seq->addRule(ft::make_unique<RangeRule>(::isalpha));
  // seq->addRule(ft::move(rep));

  seq->setDebugTag("schemeRule");
  return ft::move(seq);
}

int main(int argc, char* argv[])
{
  ft::unique_ptr<std::string> ptr(FT_NULLPTR);
  std::string s = "hello";
  take(s);
  (void)argv;
  if (argc != 2) {
    std::cerr << "Error: Usage: ./webserv [configuration file]\n";
    return 1;
  }

  try {
    const Config config = TestConfigSetup::createTestConfig();
    std::cout << config;
    ServerManager serverManager(config);

    serverManager.run();

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
