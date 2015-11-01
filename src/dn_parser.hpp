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

            // syntax as defined in rfc1779
            key          = raw[ alnum >> *(alnum | '-') ];

            dn_reserved_chars = qi::char_("\"\\+,;=%#<>");
            char_escape  = '\\' >> (hexchar | dn_reserved_chars);
            quote_string = '"' >> *(char_escape | (char_ - qi::char_("\"\\"))) >> '"' ;

            value        =  quote_string 
                         | '#' >> *hexchar
                         | *(char_escape | (char_ - dn_reserved_chars))
                         ;

            rdn_pair     = key >> '=' >> value;

            rdn          = rdn_pair % qi::char_("+");
            dn           = rdn % qi::char_(",;");

            start        = skip(qi::ascii::space) [ dn ];

            BOOST_SPIRIT_DEBUG_NODES((start)(dn)(rdn)(rdn_pair)(key)(value)(quote_string)(char_escape))
        }
        
    private:
        qi::int_parser<char, 16, 2, 2> hexchar;

        qi::rule<Iterator, ast::dn()> start;

        qi::rule<Iterator, ast::dn(), ascii::space_type> dn;
        qi::rule<Iterator, ast::rdn(), ascii::space_type> rdn;
        qi::rule<Iterator, std::pair<std::string, std::string>(), ascii::space_type> rdn_pair;

        qi::rule<Iterator, std::string()> key, value, quote_string;
        qi::rule<Iterator, char()>        char_escape, dn_reserved_chars;
    };

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
    } // namespace parsing

} // namespace pkistore
