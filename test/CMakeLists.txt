
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

enable_testing()
find_package(GTest REQUIRED)

set (SRC test_main.cc test_dn_parsing.cc)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(${PKISTORE_STATIC_INCLUDE_DIRS})
include_directories(../src)
add_executable(pkistore_test ${SRC})

set_target_properties(pkistore_test PROPERTIES CXX_STANDARD 11)
target_link_libraries(pkistore_test pthread ${GTEST_BOTH_LIBRARIES} pkistore_static)

