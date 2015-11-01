
cmake_minimum_required(VERSION 2.6)
set ( CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(pkistore CXX)

set (CXX_STANDARD_REQUIRED 1)
set (CXX_STANDARD 11)

add_subdirectory(src)
add_subdirectory(test)
