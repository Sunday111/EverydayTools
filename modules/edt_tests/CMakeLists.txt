cmake_minimum_required(VERSION 3.20)
include(set_compiler_options)
set(module_source_files
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/algorithm/algorithm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/algorithm/clamp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/algorithm/sort/bubble_sort.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/array/array_view/cast_and_conversion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/array/array_view/constructors.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/array/array_view/iteration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/bitset/bitset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/bitset/bitset_adapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/bitset/bitset_adapter_array.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/bitset/fixed_bitset_constexpr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/branchless/int_if.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/branchless/swap_if.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/guid/guid.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/math/math_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/math/matrix_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/template/all_types_match_condition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/template/any_type_match_condition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/template/apply_if.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/template/is_specialization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/template/pure_same_types.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/template/signature.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/template/static_for.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/template/types_list.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/template/values_list.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/template/worst_qualifiers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/template/worst_reference.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/private/test_tools.hpp)
add_executable(edt_tests ${module_source_files})
set_generic_compiler_options(edt_tests PRIVATE)
target_link_libraries(edt_tests PRIVATE edt
                                        gtest_main)
target_include_directories(edt_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code/public)
target_include_directories(edt_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/code/private)
enable_testing()
include(GoogleTest)
gtest_discover_tests(edt_tests)
