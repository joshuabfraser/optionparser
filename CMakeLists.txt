cmake_minimum_required(VERSION 3.10)

project(OptionParser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(OptionParser OptionParser.cpp OptionParser.h)
target_include_directories(OptionParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(options_test main.cpp)
target_link_libraries(options_test OptionParser)
