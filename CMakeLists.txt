cmake_minimum_required(VERSION 3.10)

project(options LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(options OptionParser.cpp OptionParser.h)
target_include_directories(options PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(options_test main.cpp)
target_link_libraries(options_test options)
