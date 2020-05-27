cmake_minimum_required(VERSION 3.12)
project(CHESS_ENGINE VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

juce_add_console_app(ChessEngine PRODUCT_NAME "chess-engine")

target_sources(ChessEngine PRIVATE src/main.cpp)

target_compile_definitions(ChessEngine PRIVATE JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0)

target_link_libraries(ChessEngine PRIVATE shallow-blue juce::juce_core juce::juce_events juce::juce_osc)
target_compile_options(ChessEngine PUBLIC -std=c++17 -O3 -Wall)

set_target_properties(ChessEngine PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
