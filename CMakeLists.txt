cmake_minimum_required(VERSION 3.5.1)
include(generate_vs_filters)
include(glob_cxx_sources)
include(cxx_version)

set(target_name EverydayTools)
set(EverydayTools true CACHE BOOL "Code for everyday use")
if(NOT ${${target_name}})
    return()
endif()
add_library(${target_name} INTERFACE)
target_include_directories(${target_name} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Utility project to see files in ide solution explorer
glob_cxx_sources(${CMAKE_CURRENT_SOURCE_DIR}/include target_sources)
add_custom_target("${target_name}View" SOURCES ${target_sources})
generate_vs_filters(${target_sources})

# tests
set(target_name_tests "${target_name}Tests")
# enable tests by default
set(${target_name_tests} true CACHE BOOL "Tests for ${target_name} target")
# check that user needs tests
if(NOT ${${target_name_tests}})
    return()
endif()

glob_cxx_sources(${CMAKE_CURRENT_SOURCE_DIR}/tests target_sources_tests)
add_executable(${target_name_tests} ${target_sources_tests})
generate_vs_filters(${target_sources_tests})
require_cxx_version(${target_name_tests} 17)
disable_cxx_extensions(${target_name_tests})
add_dependencies(${target_name_tests} gtest)
target_link_libraries(${target_name_tests} gtest gtest_main ${target_name})
target_include_directories(${target_name_tests} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
target_include_directories(${target_name_tests} PRIVATE ${gtest_SOURCE_DIR}/include)
target_include_directories(${target_name_tests} PRIVATE ${gtest_SOURCE_DIR})
enable_testing()
add_test(NAME    ${target_name_tests} 
         COMMAND ${target_name_tests})

if(MSVC)
    target_compile_options(${target_name_tests} PRIVATE "/bigobj")
endif()
