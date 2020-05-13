cmake_minimum_required(VERSION 3.0.0)
project(chess-engine VERSION 0.1.0)

set(CMAKE_CXX_COMPILER "/usr/local/Cellar/gcc/9.3.0_1/bin/g++-9")


enable_testing()

file(GLOB shallow_blue_src "src/shallow-blue-fork/src/*.cc")
file(GLOB interface_src "src/*.cpp")
file(GLOB tinyosc_src "src/tinyosc/tinyosc.c")

add_library(tiny_osc OBJECT ${tinyosc_src})
add_executable(chess-engine $<TARGET_OBJECTS:tiny_osc> ${shallow_blue_src} ${interface_src})
add_executable(chess-engine-test $<TARGET_OBJECTS:tiny_osc> ${shallow_blue_src} "src/tests/chess_engine_tests.cpp")
add_executable(tiny-osc-test $<TARGET_OBJECTS:tiny_osc> "src/tests/tiny_osc_tests.cpp")

target_compile_options(chess-engine PUBLIC -std=c++17 -O3 -Wall)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

