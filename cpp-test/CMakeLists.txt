cmake_minimum_required(VERSION 3.6)
project(cpp_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp test-reference/test-reference.cpp)
file(GLOB source_file  "F:/code/github/chenzhongtao/mysource/C++reference/containers/*.cpp")
set(SOURCE_FILES ${source_file})
file(GLOB source_file2  "F:/code/github/chenzhongtao/mysource/codingInterview/*.cpp")
set(SOURCE_FILES ${source_file2})
add_executable(cpp_test ${SOURCE_FILES})