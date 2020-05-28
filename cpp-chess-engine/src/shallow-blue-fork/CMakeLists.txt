cmake_minimum_required(VERSION 3.12)

file(GLOB shallow-blue-src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(shallow-blue STATIC ${shallow-blue-src})

target_include_directories(shallow-blue PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})