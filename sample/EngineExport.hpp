#pragma once 

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define DECL_EXPORT __declspec(dllexport)
#  define DECL_IMPORT __declspec(dllimport)
#else
#  define DECL_EXPORT     __attribute__((visibility("default")))
#  define DECL_IMPORT     __attribute__((visibility("default")))
#endif

#if defined(ENGINE_LIBRARY)
#  define ENGINE_EXPORT DECL_EXPORT
#else
#  define ENGINE_EXPORT DECL_IMPORT
#endif

/* type_init function that creates Type object for all types. */
extern "C" {
    void ENGINE_EXPORT type_init();
    void ENGINE_EXPORT type_clear();
}