cmake_minimum_required(VERSION 3.24 FATAL_ERROR)
project(tiko VERSION 1.0 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(Td
    GIT_REPOSITORY https://github.com/tdlib/td.git
)

FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1
)

FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_Declare(httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.18.1
)

FetchContent_Declare(cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.4.2
)

FetchContent_MakeAvailable(Td spdlog nlohmann_json httplib cli11)

include_directories("${PROJECT_SOURCE_DIR}")

add_executable(tiko tiko.cpp)
add_executable(tgen tgen.cpp)

add_subdirectory(tg)
add_subdirectory(diverse)
add_subdirectory(utils)

target_link_libraries(tiko PRIVATE Td::TdJsonStatic)
target_link_libraries(tiko PRIVATE nlohmann_json)
target_link_libraries(tiko PRIVATE "$<LINK_LIBRARY:WHOLE_ARCHIVE,tg_manual>")
target_link_libraries(tiko PRIVATE utils)
target_link_libraries(tiko PRIVATE spdlog)

target_link_libraries(tgen PRIVATE Td::TdJsonStatic)
target_link_libraries(tgen PRIVATE nlohmann_json)
target_link_libraries(tgen PRIVATE CLI11::CLI11)
target_link_libraries(tgen PRIVATE "$<LINK_LIBRARY:WHOLE_ARCHIVE,tg_auto>")
target_link_libraries(tgen PRIVATE utils)
target_link_libraries(tgen PRIVATE spdlog)

set_property(TARGET tiko PROPERTY CXX_STANDARD 20)
set_property(TARGET tgen PROPERTY CXX_STANDARD 20)