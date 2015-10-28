//#define BOOST_SPIRIT_DEBUG
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/spirit/include/qi.hpp>
#include <map>
#include <set>

namespace pkistore {
    namespace parsing {

    namespace qi      = boost::spirit::qi;
    namespace ascii   = boost::spirit::ascii;

    namespace ast {
        typedef std::map<std::string, std::string> rdn;
        typedef std::vector<rdn> dn;
    }

    template <typename Iterator>
    struct dn_grammar_common : public qi::grammar<Iterator, ast::dn()> {
        dn_grammar_common() : dn_grammar_common::base_type(start) {
            using namespace qi;

            // Low level rules
            // Key can only contain alphanumerical characters and dashes
            key          = raw[ alnum >> *(alnum | '-') ];

            // Rule for a fully escaped string (used as Attribute Value) => "..."
            char_escape  = '\\' >> (hexchar | dn_reserved_chars);
            quote_string = '"' >> *(char_escape | (char_ - dn_reserved_chars)) >> '"' ;

            // Value is either:
            // - A regular string (that can contain escaped sequences)
            // - A fully escaped string (that can also contain escaped sequences)
            // - An hexadecimal string
            value =  quote_string 
                  | '#' >> *hexchar
                  | *(char_escape | (char_ - dn_reserved_chars))
                  ;

            // Higher level rules
            rdn_pair = key >> '=' >> value;

            // A relative distinguished name consists of a sequence of pairs (Attribute = AttributeValue)
            // Separated with a +
            rdn = rdn_pair % qi::char_("+");

            // The DN is a set of RDNs separated by either a "," or a ";".
            // The two separators can coexist in a given DN, though it is not
            // recommended practice.
            dn = rdn % qi::char_(",;");

            start = skip(qi::ascii::space) [ dn ];

            BOOST_SPIRIT_DEBUG_NODES((start)(dn)(rdn)(rdn_pair)(key)(value)(quote_string)(char_escape))
        }
        
    private:
        qi::int_parser<char, 16, 2, 2> hexchar;

        qi::rule<Iterator, ast::dn()> start;

        qi::rule<Iterator, ast::dn(), ascii::space_type> dn;
        qi::rule<Iterator, ast::rdn(), ascii::space_type> rdn;
        qi::rule<Iterator, std::pair<std::string, std::string>(), ascii::space_type> rdn_pair;

        qi::rule<Iterator, std::string()> key, value, quote_string;
        qi::rule<Iterator, char()>        char_escape;

        struct dn_reserved_chars_ : public qi::symbols<char, char> {
            dn_reserved_chars_() {
                add ("\\", '\\') ("\"", '"')
                    ("=" , '=')  ("+" , '+')
                    ("," , ',')  (";" , ';')
                    ("#" , '#')  ("%" , '%')
                    ("<" , '<')  (">" , '>')
                    ;
            }
        } dn_reserved_chars;
    };

    } // namespace parsing

    static parsing::ast::dn parse(std::string const& input) {
        using It = std::string::const_iterator;

        pkistore::parsing::dn_grammar_common<It> const g;

        It f = input.begin(), l = input.end();
        pkistore::parsing::ast::dn parsed;

        bool ok = boost::spirit::qi::parse(f, l, g, parsed);

        if (!ok || (f!=l))
            throw std::runtime_error("dn_parse failure");

        return parsed;
    }
} // namespace pkistore
