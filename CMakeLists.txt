cmake_minimum_required(VERSION 3.10)

project(pack)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

set(BUILD_GMOCK OFF CACHE BOOL "Not building GMock")

add_subdirectory(googletest)
include_directories(googletest/include)

add_executable(packtest tests/test.cpp include/pack.h)
target_link_libraries(packtest gtest)
