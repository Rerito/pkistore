
#include <boost/spirit/include/qi.hpp>
#include <unordered_map>
#include <set>

namespace pkistore {
namespace parsing {

namespace qi = boost::spirit::qi;

typedef std::unordered_map<std::string, std::string> dn_key_value_map;

template <typename Iterator>
struct dn_grammar_common : public qi::grammar<Iterator, std::multiset<dn_key_value_map>()> {
  dn_grammar_common() : dn_grammar_common::base_type(dn) {
    struct dn_reserved_chars_ : public qi::symbols<char> {
      dn_reserved_chars_() {
        add
          ("\\", "\\")
          ("=" , "=")
          ("+" , "+")
          ("," , ",")
          (";" , ";")
          ("#" , "#")
          ("<" , "<")
          (">" , ">")
          ("\"", "\"")
          ("%" , "%");
      }
    } dn_reserved_chars;
    dn = rdn % (qi::char_(",;"));
    rdn_pair = key >> -('=' >> value);
    key = (*qi::char_("a-zA-Z"));
    hex_string = (&qi::char_("#")) >> *(qi::repeat(2)[qi::char_("0-9a-fA-F")]);
    value = (qi::lexeme[*(qi::char_("a-zA-Z0-9") | (&qi::char_("\\") >> dn_reserved_chars))]) |
            ((&qi::char_('"')) >> qi::lexeme[*(qi::char_("0-9a-zA-Z") | dn_reserved_chars)] >> (&qi::char_('"'))) |
            hex_string;
  }
  qi::rule<Iterator, std::multiset<dn_key_value_map>()> dn;
  qi::rule<Iterator, dn_key_value_map()> rdn;
  qi::rule<Iterator, std::pair<std::string, std::string>()> rdn_pair;
  qi::rule<Iterator, std::string()> key, value, hex_string;
};

} // namespace parsing
} // namespace pkistore
