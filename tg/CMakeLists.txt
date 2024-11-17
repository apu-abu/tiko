cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

add_library(tg_auto client.cpp routine.cpp routine/authorization.cpp)
add_library(tg_manual client.cpp routine.cpp routine/authorization.cpp)

target_link_libraries(tg_auto PRIVATE Td::TdJson)
target_link_libraries(tg_auto PRIVATE nlohmann_json)
target_link_libraries(tg_auto PRIVATE auto)
target_link_libraries(tg_auto PRIVATE spdlog)

target_link_libraries(tg_manual PRIVATE Td::TdJson)
target_link_libraries(tg_manual PRIVATE nlohmann_json)
target_link_libraries(tg_manual PRIVATE manual)
target_link_libraries(tg_manual PRIVATE spdlog)

set_property(TARGET tg_auto PROPERTY CXX_STANDARD 20)
set_property(TARGET tg_manual PROPERTY CXX_STANDARD 20)