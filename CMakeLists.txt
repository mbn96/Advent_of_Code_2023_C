cmake_minimum_required(VERSION 3.26)
project(AdventOfCode2023 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)
add_link_options(-lm)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-g -O0)
else()
  add_compile_options(-O3)
endif()

set(COMMON_SOURCES "./defs.h" "./comm_io.c" "./str_utils.c" "./hash_map.c")
add_library(common STATIC ${COMMON_SOURCES})

add_executable(day1 "./d1/d1.c")
target_link_libraries(day1 common)

add_executable(day2 "./d2/d2.c")
target_link_libraries(day2 common)

add_executable(day3 "./d3/d3.c")
target_link_libraries(day3 common)

add_executable(day4 "./d4/d4.c")
target_link_libraries(day4 common)
