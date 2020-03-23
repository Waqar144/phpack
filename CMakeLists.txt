cmake_minimum_required(VERSION 3.10)

project(pack)

add_subdirectory(googletest)
include_directories(googletest/include)

add_executable(main main.cpp
                        src/pack.h
                        src/pack.cpp)

add_executable(packtest tests/test.cpp src/pack.h src/pack.cpp)
target_link_libraries(packtest gtest)
