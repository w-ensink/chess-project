cmake_minimum_required(VERSION 3.12)

add_subdirectory(shallow-blue-fork)
add_subdirectory(tinyosc)

add_executable(chess-engine main.cpp
                            MoveBuilder.cpp
                            MoveBuilderTester.cpp
                            MoveBuilderTester.h
                            ChessEngine.cpp
                            ChessEngine.h
                            ChessEngineTester.cpp
                            ChessEngineTester.h)

target_link_libraries(chess-engine PUBLIC tiny-osc shallow-blue -pthread)
target_compile_options(chess-engine PUBLIC -std=c++17 -Wall -Wpedantic)