
#include <boost/spirit/include/qi.hpp>
#include <unordered_map>
#include <set>

namespace pkistore {
namespace parsing {

namespace qi = boost::spirit::qi;

typedef std::unordered_map<std::string, std::string> dn_key_value_map;

template <typename Iterator>
struct dn_grammar_common : public qi::grammar<Iterator, std::multiset<dn_key_value_map>()> {
  struct dn_reserved_chars_ : public qi::symbols<char, char> {
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
  dn_grammar_common() : dn_grammar_common::base_type(dn) {
    dn = rdn % (qi::char_(",;"));
    rdn = rdn_pair % qi::char_("+");
    rdn_pair = key >> -('=' >> value);
    key = (*qi::char_("a-zA-Z"));
    escaped_hex_char = qi::lexeme[(&qi::char_("\\")) >> qi::repeat(2)[qi::char_("0-9a-fA-F")]];
    escaped_sequence = escaped_hex_char |
                      qi::lexeme[(&qi::char_("\\")) >> dn_reserved_chars];
    quote_string = (&qi::char_('"')) >> qi::lexeme[*(escaped_sequence | (qi::char_ - qi::char_("\\\"")))] >> (&qi::char_('"'));
    hex_string = (&qi::char_("#")) >> *(qi::repeat(2)[qi::char_("0-9a-fA-F")]);
    value = (qi::lexeme[*((qi::char_ - dn_reserved_chars) | escaped_sequence)]) |
            quote_string |
            hex_string;
  }
  qi::rule<Iterator, std::multiset<dn_key_value_map>()> dn;
  qi::rule<Iterator, dn_key_value_map()> rdn;
  qi::rule<Iterator, std::pair<std::string, std::string>()> rdn_pair;
  qi::rule<Iterator, std::string()> key, value, hex_string, quote_string;
  qi::rule<Iterator, std::string()> escaped_hex_char, escaped_sequence;
};

} // namespace parsing
} // namespace pkistore
