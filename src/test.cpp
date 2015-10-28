#include "dn_parser.hpp"

int main() {
    for (std::string const input : {
            "OU=Sales + CN=J. Smith, O=Widget Inc., C=US",
            "OU=#53616c6573",
            "OU=Sa\\+les + CN=J. Smi\\%th, O=Wid\\,\\;get In\\3bc., C=US",
            //"CN=Marshall T. Rose, O=Dover Beach Consulting, L=Santa Clara,\nST=California, C=US",
            //"CN=FTAM Service, CN=Bells, OU=Computer Science,\nO=University College London, C=GB",
            //"CN=Markus Kuhn, O=University of Erlangen, C=DE",
            //"CN=Steve Kille,\nO=ISODE Consortium,\nC=GB",
            //"CN=Steve Kille ,\n\nO =   ISODE Consortium,\nC=GB",
            //"CN=Steve Kille, O=ISODE Consortium, C=GB\n",
        })
    {
        auto parsed = pkistore::parse(input);

        std::cout << "===========\n" << input << "\n";
        for(auto const& dn : parsed) {
            std::cout << "-----------\n";
            for (auto const& kv : dn) {
                std::cout << "\t" << kv.first << "\t->\t" << kv.second << "\n";
            }
        }
    }
}
