cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS "*.cpp" "routine/*.cpp")

add_library(tg ${GLOB_RECURSE})
add_library(tg_auto ${GLOB_RECURSE})
add_library(tg_manual ${GLOB_RECURSE})

target_link_libraries(tg PRIVATE Td::TdJson)
target_link_libraries(tg PRIVATE nlohmann_json)
target_link_libraries(tg PRIVATE natural)
target_link_libraries(tg PRIVATE spdlog)
target_link_libraries(tg PRIVATE dao)

target_link_libraries(tg_auto PRIVATE Td::TdJson)
target_link_libraries(tg_auto PRIVATE nlohmann_json)
target_link_libraries(tg_auto PRIVATE auto)
target_link_libraries(tg_auto PRIVATE spdlog)
target_link_libraries(tg_auto PRIVATE dao)

target_link_libraries(tg_manual PRIVATE Td::TdJson)
target_link_libraries(tg_manual PRIVATE nlohmann_json)
target_link_libraries(tg_manual PRIVATE manual)
target_link_libraries(tg_manual PRIVATE spdlog)
target_link_libraries(tg_manual PRIVATE dao)

set_property(TARGET tg PROPERTY CXX_STANDARD 20)
set_property(TARGET tg_auto PROPERTY CXX_STANDARD 20)
set_property(TARGET tg_manual PROPERTY CXX_STANDARD 20)