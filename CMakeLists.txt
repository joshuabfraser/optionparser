cmake_minimum_required(VERSION 3.10)

project(OptionParser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(OptionParser OptionParser.cpp OptionParser.h)
target_include_directories(OptionParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(example example.cpp)
target_link_libraries(example OptionParser)
