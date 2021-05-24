cmake_minimum_required(VERSION 3.15)
project(Vector)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(googletest)
include_directories(googletest/googletest/include)
include_directories(googletest/googlemock/include)

add_executable(Vector main.cpp Vector.h tests.cpp)
target_link_libraries(Vector gtest gtest_main)