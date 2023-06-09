cmake_minimum_required(VERSION 3.10)

project(options LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(options OptionParser.cpp OptionParser.h)

add_executable(options_test main.cpp)
target_link_libraries(options_test options)
