
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

set (SRC dn_parser.hpp)

add_library(pkistore_static ${SRC})
set_target_properties(pkistore_static PROPERTIES LINKER_LANGUAGE CXX)

