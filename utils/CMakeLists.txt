cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

add_library(utils ada.cpp logger.cpp schedule.cpp variable.cpp database.cpp)

target_link_libraries(utils PRIVATE spdlog)

set_property(TARGET utils PROPERTY CXX_STANDARD 20)