cmake_minimum_required(VERSION 3.16)
include(generic_compile_options)

set(target_name EverydayToolsTests)
file(GLOB_RECURSE target_sources "*.cpp")
add_executable(${target_name} ${target_sources})
set_generic_compile_options(${target_name} PRIVATE)
add_dependencies(${target_name} gtest)
target_link_libraries(${target_name} gtest gtest_main EverydayTools)
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
enable_testing()
add_test(NAME ${target_name}
    COMMAND ${target_name})

if(MSVC)
    target_compile_options(${target_name} PRIVATE "/bigobj")
endif()
