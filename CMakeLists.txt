cmake_minimum_required(VERSION 3.10)

project(pack)

add_subdirectory(googletest)
include_directories(googletest/include)

add_executable(main main.cpp
                        pack.h
                        pack.cpp)

add_executable(packtest test.cpp pack.cpp)
target_link_libraries(packtest gtest)
