cmake_minimum_required(VERSION 3.0.0)

project(chess-engine VERSION 1.0.0)

add_subdirectory(src)
set_target_properties(chess-engine PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")