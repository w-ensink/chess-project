
set(CMAKE_CXX_COMPILER "/usr/local/Cellar/gcc/9.3.0_1/bin/g++-9")
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(shallow-blue-fork)
add_subdirectory(tinyosc)

#file(GLOB shallow_blue_src "src/shallow-blue-fork/src/*.cc")
#file(GLOB interface_src "src/*.cpp")
#file(GLOB tinyosc_src "tinyosc/tinyosc.c")

#add_library(tiny_osc OBJECT ${tinyosc_src})
#add_executable(../chess-engine $<TARGET_OBJECTS:tiny_osc> ${shallow_blue_src} ${interface_src})
#add_executable(../chess-engine-test $<TARGET_OBJECTS:tiny_osc> ${shallow_blue_src} "tests/chess_engine_tests.cpp")
#add_executable(../tiny-osc-test $<TARGET_OBJECTS:tiny_osc> "tests/tiny_osc_tests.cpp")

add_executable(chess-engine main.cpp)
target_link_libraries(chess-engine PUBLIC shallow-blue tiny-osc)
target_compile_options(chess-engine PUBLIC -std=c++17 -O3 -Wall)


