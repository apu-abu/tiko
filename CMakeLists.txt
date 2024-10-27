cmake_minimum_required(VERSION 3.24 FATAL_ERROR)
project(tiko VERSION 1.0 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(Td
    GIT_REPOSITORY https://github.com/tdlib/td.git
)
FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(Td nlohmann_json)

include_directories("${PROJECT_SOURCE_DIR}")
add_executable(tiko tiko.cpp)

add_subdirectory(tg)

# add_subdirectory(utils)
target_link_libraries(tiko PRIVATE Td::TdJson)
target_link_libraries(tiko PRIVATE nlohmann_json)
target_link_libraries(tiko PRIVATE tg)

set_property(TARGET tiko PROPERTY CXX_STANDARD 20)