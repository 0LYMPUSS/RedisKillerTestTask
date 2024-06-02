cmake_minimum_required(VERSION 3.11)
project(RedisKillerServer)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_VERBOSE_MAKEFILE ON)

if (NOT PKG_CONFIG_FOUND)
    include (CMakeFindDependencyMacro)
    find_dependency(PkgConfig)
endif ()

include(FetchContent)

FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/asio
    GIT_TAG asio-1-30-2
    )
FetchContent_MakeAvailable(asio)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/spdlog
    GIT_TAG v1.x
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/json
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

enable_testing()

add_subdirectory(server_test)


file (GLOB SRC 
        "server/src/*.cpp"
        "server/include/*.h"
    )

include_directories(server/include asio/asio/include spdlog/include json/include)

add_library(server_lib ${SRC})

add_executable(r_server server/redis_killer_server.cpp)
add_executable(r_client client/redis_killer_client.cpp)

target_link_libraries(r_server PRIVATE server_lib)
