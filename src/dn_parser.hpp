
#include <boost/spirit/include/qi.hpp>
#include <unordered_map>
#include <set>

namespace pkistore {
namespace parsing {

namespace qi = boost::spirit::qi;

typedef std::unordered_map<std::string, std::string> dn_key_value_map;

template <typename Iterator>
struct dn_grammar : public qi::grammar<Iterator, std::multiset<dn_key_value_map>()> {
  dn_grammar() : dn_grammar::base_type(dn) {
    struct dn_reserved_chars : public qi::symbols<char> {
    };
    dn = rdn % ',';
    rdn_pair = key >> -('=' >> value);
    key = *qi::char_("a-zA-Z");
    value = *(qi::char_("a-zA-Z0-9") /*| (qi::char_("\\") >> dn_reserved_chars)*/);
  }
  qi::rule<Iterator, std::multiset<dn_key_value_map>()> dn;
  qi::rule<Iterator, dn_key_value_map()> rdn;
  qi::rule<Iterator, std::pair<std::string, std::string>()> rdn_pair;
  qi::rule<Iterator, std::string()> key, value;
};

} // namespace parsing
} // namespace pkistore
