
enable_testing()
find_package(GTest REQUIRED)

set (CXX_STANDARD 11)
set (SRC test_main.cc test_dn_parsing.cc)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${PKISTORE_STATIC_INCLUDE_DIRS})
include_directories(../src)
add_executable(pkistore_test ${SRC})
target_link_libraries(pkistore_test GTEST_BOTH_LIBRARIES pkistore_static)

