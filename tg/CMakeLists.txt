cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

add_library(tg client.cpp command.cpp)

target_link_libraries(tg PRIVATE Td::TdJson)
target_link_libraries(tg PRIVATE nlohmann_json)
set_property(TARGET tg PROPERTY CXX_STANDARD 20)