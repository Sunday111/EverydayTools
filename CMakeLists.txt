cmake_minimum_required(VERSION 3.20)

project(EverydayTools LANGUAGES CXX)

file(GLOB_RECURSE edt_headers CONFIGURE_DEPENDS
    "modules/edt/code/public/*.hpp")

add_library(edt INTERFACE)
target_sources(edt INTERFACE ${edt_headers})
target_include_directories(edt INTERFACE modules/edt/code/public)
target_compile_features(edt INTERFACE cxx_std_20)

option(EDT_WITH_TESTS "Build EverydayTools library tests" OFF)
if(EDT_WITH_TESTS)
    include(FetchContent)

    option(EDT_FETCH_GOOGLE_TESTS "Disable it if you already have google tests in your project" ON)
    option(EDT_FIND_GTEST_PACKAGE "Enable to make the script look for google tests in the system" ON)

    if(EDT_FETCH_GOOGLE_TESTS)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest
            GIT_TAG        "v1.17.0"
        )
        FetchContent_MakeAvailable(googletest)
    elseif(EDT_FIND_GTEST_PACKAGE)
        find_package(GTest REQUIRED)
        add_library(gtest ALIAS GTest::gtest)
        add_library(gtest_main ALIAS GTest::gtest_main)
    endif()

    file(GLOB_RECURSE edt_test_sources CONFIGURE_DEPENDS
        "modules/EverydayToolsTests/code/private/*.cpp"
        "modules/EverydayToolsTests/code/private/*.hpp")

    add_executable(EverydayToolsTests ${edt_test_sources})
    target_link_libraries(EverydayToolsTests PRIVATE edt gtest_main)
    target_include_directories(EverydayToolsTests PRIVATE modules/EverydayToolsTests/code/private)
    target_compile_features(EverydayToolsTests PRIVATE cxx_std_20)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        option(EDT_TESTS_WITH_COVERAGE "Instrument test executable with clang's coverage data" ON)
        if(EDT_TESTS_WITH_COVERAGE)
            set(coverage_flags -fprofile-instr-generate -fcoverage-mapping -fcoverage-mcdc)
            target_compile_options(EverydayToolsTests PRIVATE ${coverage_flags})
            target_link_options(EverydayToolsTests PRIVATE ${coverage_flags})
        endif()
    endif()

    if(MSVC)
        target_compile_options(EverydayToolsTests PRIVATE "/bigobj")
    endif()

    enable_testing()
    include(GoogleTest)
    gtest_discover_tests(EverydayToolsTests)
endif()
