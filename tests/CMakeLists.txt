cmake_minimum_required(VERSION 3.16)

include(FetchContent)

option(EDT_FETCH_GOOGLE_TESTS "Disable it if you already have google tests in your project" ON)
option(EDT_FIND_GTEST_PACKAGE "Enable to make the script look for google tests in the system" ON)

if (EDT_FETCH_GOOGLE_TESTS)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG        "release-1.12.1"
    )
    
  FetchContent_MakeAvailable(googletest)
elseif(EDT_FIND_GTEST_PACKAGE)
  find_package(GTest REQUIRED)
  add_library(gtest ALIAS GTest::gtest)
  add_library(gtest_main ALIAS GTest::gtest_main)
endif()

set(target_name EverydayToolsTests)
file(GLOB_RECURSE target_sources "*.cpp")
add_executable(${target_name} ${target_sources})
add_dependencies(${target_name} gtest)
set_property(TARGET ${target_name} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${target_name} PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(${target_name} gtest gtest_main edt)
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  option(EDT_TESTS_WITH_COVERAGE "Instrument test executable with clang's coverage data" ON)
  if (EDT_TESTS_WITH_COVERAGE)
    set(coverage_flags -fprofile-instr-generate -fcoverage-mapping -fcoverage-mcdc)
    target_compile_options(${target_name} PRIVATE ${coverage_flags})
    target_link_options(${target_name} PRIVATE ${coverage_flags})
  endif()
endif()

enable_testing()
include(GoogleTest)
gtest_discover_tests(${target_name})
add_test(NAME ${target_name} COMMAND ${target_name})

if(MSVC)
    target_compile_options(${target_name} PRIVATE "/bigobj")
endif()
