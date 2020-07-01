cmake_minimum_required(VERSION 3.10)

project(pack)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(googletest)
include_directories(googletest/include)

add_executable(main main.cpp
    include/pack.h)

add_executable(packtest tests/test.cpp include/pack.h)
target_link_libraries(packtest gtest)
