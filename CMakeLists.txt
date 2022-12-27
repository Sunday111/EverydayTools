cmake_minimum_required(VERSION 3.16)

project(EverydayTools)
include(generic_compile_options)

set(target_name EverydayTools)
add_library(${target_name} INTERFACE)
set_generic_compile_options(${target_name} INTERFACE)
target_include_directories(${target_name} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

option(EVERYDAY_TOOLS_WITH_TESTS "Build EverydayTools library tests" OFF)
if (EVERYDAY_TOOLS_WITH_TESTS)
    add_subdirectory(tests)
endif()