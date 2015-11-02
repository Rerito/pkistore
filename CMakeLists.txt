
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
set ( CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(pkistore CXX)

set (CXX_STANDARD_REQUIRED 1)
set (CXX_STANDARD 11)

add_subdirectory(src)
add_subdirectory(test)
