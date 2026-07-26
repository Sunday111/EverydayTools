cmake_minimum_required(VERSION 3.20)
include(set_compiler_options)
set(module_source_files
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Algorithm/Algorithm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Algorithm/Clamp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Algorithm/Sort/BubbleSort.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Array/ArrayView/CastAndConversion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Array/ArrayView/Constructors.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Array/ArrayView/Iteration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Bitset/Bitset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Bitset/BitsetAdapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Bitset/BitsetArrayAdapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Bitset/FixedBitsetConstexpr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Branchless/IntIf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Branchless/SwapIf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/GUID/GUID.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Math/math_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Math/matrix_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Template/AllTypesMatchCondition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Template/AnyTypeMatchCondition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Template/ApplyIf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Template/IsSpecialization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Template/PureSameTypes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Template/Signature.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Template/StaticFor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Template/TypesList.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Template/ValuesList.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Template/WorstQualifiers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/Template/WorstReference.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/TestTools.hpp)
add_executable(EverydayToolsTests ${module_source_files})
set_generic_compiler_options(EverydayToolsTests PRIVATE)
target_link_libraries(EverydayToolsTests PRIVATE edt
                                                 gtest_main)
target_include_directories(EverydayToolsTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code/public)
target_include_directories(EverydayToolsTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/code/private)
enable_testing()
include(GoogleTest)
gtest_discover_tests(EverydayToolsTests)
