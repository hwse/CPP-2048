cmake_minimum_required(VERSION 3.16)
project(game_2048 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

# Boost Setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_executable(game WIN32 main.cpp)
target_link_libraries(game PUBLIC CONAN_PKG::wxwidgets)