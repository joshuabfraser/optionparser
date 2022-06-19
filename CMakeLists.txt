cmake_minimum_required(VERSION 3.10)

project(options LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(options main.cpp OptionParser.cpp OptionParser.h)
