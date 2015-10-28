
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/set.hpp>
#include <boost/spirit/include/qi.hpp>
#include <unordered_map>
#include <set>

namespace pkistore {
namespace parsing {

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

typedef std::unordered_map<std::string, std::string> dn_key_value_map;

template <typename Iterator>
struct dn_grammar_common : public qi::grammar<Iterator, std::multiset<dn_key_value_map>(), ascii::space_type> {
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
    // Useful using directives
    using namespace qi::labels;

    // Low level rules
    // Key can only contain alphanumerical characters and dashes
    key = ascii::no_case[qi::lexeme[(*qi::alnum) >> (*(qi::char_('-') >> qi::alnum))]];
    escaped_hex_char = qi::lexeme[(&qi::char_("\\")) >> qi::repeat(2)[qi::char_("0-9a-fA-F")]];
    escaped_sequence = escaped_hex_char |
                      qi::lexeme[(&qi::char_("\\")) >> dn_reserved_chars];
    // Rule for a fully escaped string (used as Attribute Value) => "..."
    quote_string = qi::lexeme[qi::lit('"') >>
      *(escaped_sequence | (qi::char_ - qi::char_("\\\""))) >>
      qi::lit('"')
    ];
    // Rule for an hexa string (used as Attribute Value) => #23AD5D...
    hex_string = (&qi::char_("#")) >> *qi::lexeme[(qi::repeat(2)[qi::char_("0-9a-fA-F")])];

    // Value is either:
    // - A regular string (that can contain escaped sequences)
    // - A fully escaped string (that can also contain escaped sequences)
    // - An hexadecimal string
    value = (qi::lexeme[*((qi::char_ - dn_reserved_chars) | escaped_sequence)]) |
            quote_string |
            hex_string;

    // Higher level rules
    rdn_pair = key >> '=' >> value;
    // A relative distinguished name consists of a sequence of pairs (Attribute = AttributeValue)
    // Separated with a +
    rdn = rdn_pair % qi::char_("+");
    // The DN is a set of RDNs separated by either a "," or a ";".
    // The two separators can coexist in a given DN, though it is not
    // recommended practice.
    dn = rdn % (qi::char_(",;"));
  }
  qi::rule<Iterator, std::set<dn_key_value_map>(), ascii::space_type> dn;
  qi::rule<Iterator, dn_key_value_map(), ascii::space_type> rdn;
  qi::rule<Iterator, std::pair<std::string, std::string>(), ascii::space_type> rdn_pair;
  qi::rule<Iterator, std::string(), ascii::space_type> key, value, hex_string, quote_string;
  qi::rule<Iterator, std::string(), ascii::space_type> escaped_hex_char, escaped_sequence;
};

} // namespace parsing
} // namespace pkistore
