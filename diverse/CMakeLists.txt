cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

add_library(auto auto/authorize.cpp)
add_library(manual manual/authorize.cpp)
add_library(natural authorize.cpp)

target_link_libraries(auto PRIVATE httplib)
target_link_libraries(auto PRIVATE utils)

set_property(TARGET auto PROPERTY CXX_STANDARD 20)
set_property(TARGET manual PROPERTY CXX_STANDARD 20)
set_property(TARGET natural PROPERTY CXX_STANDARD 20)