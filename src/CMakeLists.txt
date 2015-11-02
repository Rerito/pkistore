
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

find_package(Boost 1.54 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set (SRC dn_parser.hpp)

add_library(pkistore_static ${SRC})
set_target_properties(pkistore_static PROPERTIES LINKER_LANGUAGE CXX)

