#pragma once

#ifdef _MSC_VER
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#endif

#ifdef __GNUC__
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT __attribute__((visibility("default")))
#endif

#ifdef __clang__
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT __attribute__((visibility("default")))
#endif

