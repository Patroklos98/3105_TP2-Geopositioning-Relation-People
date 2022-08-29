cmake_minimum_required(VERSION 3.16)
project(INF3105TP2)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/8d51dc50eb7e7698427fed81b85edad0e032112e.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

## tp2
add_executable(tp2 tp2.cpp tp2.h)
target_sources(tp2 PUBLIC "./personne.cpp")
target_sources(tp2 PUBLIC "./coordonnees.cpp")

## gtest_tableau
enable_testing()

add_executable(gtest_tableau gtest_tableau.cpp)

add_executable(gtest_coordonnees gtest_coordonnees.cpp)
target_sources(gtest_coordonnees PUBLIC "./coordonnees.cpp")

target_link_libraries(gtest_tableau gtest_main)
target_link_libraries(gtest_coordonnees gtest_main)

include(GoogleTest)
gtest_discover_tests(gtest_tableau)
gtest_discover_tests(gtest_coordonnees)
