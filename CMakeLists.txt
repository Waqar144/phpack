cmake_minimum_required(VERSION 3.12)

project(pack)

if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

set(BUILD_GMOCK OFF CACHE BOOL "Not building GMock")

add_subdirectory(googletest)
include_directories(googletest/include)

############
add_library(project_options INTERFACE)

if(${CMAKE_VERSION} VERSION_LESS "3.12.4")
        message("Please consider to switch to CMake >= 3.12.4 to enable sanitizers")
else()
    include(cmake/sanitizers.cmake)
    enable_sanitizers(project_options)
endif()
############

add_library(project_warnings INTERFACE)

# standard compiler warnings
include(cmake/warnings.cmake)
set_project_warnings(project_warnings)

add_executable(packtest tests/test.cpp include/pack.h include/pack_impl.h)

target_link_libraries(packtest project_warnings)
target_link_libraries(packtest project_options)
target_link_libraries(packtest gtest)
