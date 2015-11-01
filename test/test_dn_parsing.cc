
#include "gtest/gtest.h"
#include "dn_parser.hpp"

TEST(DnParserTest, BasicDn) {
  using namespace pkistore::parsing;
  std::string basic_dn("OU=Sales, CN=John Smith, O=Dummy Inc., C=US");
  ast::dn expected_dn = {{{"OU", "Sales"}}, {{"CN", "John Smith"}},
                        {{"O", "Dummy Inc."}}, {{"C", "US"}}};
  ast::dn parsed_dn = parse(basic_dn);
  ASSERT_FALSE(parsed_dn.empty()) << "Parsing is not empty";
  ASSERT_EQ(parsed_dn, expected_dn);
}
