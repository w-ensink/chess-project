# Building Tiny OSC as static lib

add_library(tiny-osc STATIC tinyosc.c tinyosc.h osc_interface.h)

target_include_directories(tiny-osc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
