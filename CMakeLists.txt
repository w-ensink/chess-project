cmake_minimum_required(VERSION 3.0.0)
project(chess-engine VERSION 0.1.0)

set(CMAKE_CXX_COMPILER "/usr/local/Cellar/gcc/9.3.0_1/bin/g++-9")


enable_testing()

file(GLOB shallow_blue_src "src/shallow-blue-fork/src/*.cc")
file(GLOB interface_src "src/*.cpp")

add_executable(chess-engine ${shallow_blue_src} ${interface_src})
add_executable(chess-engine-test ${shallow_blue_src} "src/tests/test.cpp")
target_compile_options(chess-engine PUBLIC -std=c++17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

