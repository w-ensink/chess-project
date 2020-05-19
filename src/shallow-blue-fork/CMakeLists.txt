# CMake File for building Shallow Blue Engine as static lib

file(GLOB shallow-blue-src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_library(shallow-blue STATIC shallow_blue_interface.hpp ${shallow-blue-src})

target_include_directories(shallow-blue PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

