cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS "*.cpp")
add_library(dao OBJECT ${SOURCE})

target_link_libraries(dao PRIVATE utils pqxx spdlog)

set_property(TARGET dao PROPERTY CXX_STANDARD 20)