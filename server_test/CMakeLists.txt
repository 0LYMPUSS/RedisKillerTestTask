
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/google/googletest
  GIT_TAG main
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB SRC 
    "src/*.cpp"
    "src/*.h"
    )

include_directories(../server/include googletest/include ../json/include ../spdlog/include)

add_executable(server_test ${SRC})

target_link_libraries(server_test PRIVATE gtest gtest_main server_lib)

include(GoogleTest)
gtest_discover_tests(server_test)