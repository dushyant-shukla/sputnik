#pragma once

#include "core/logging/logging_core.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// TYPE DECORATORS /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * A type decorator to make the type non-copyable
 */
#define NON_COPYABLE(T)               \
    T(const T&)             = delete; \
    T(const T&&)            = delete; \
    T& operator=(const T&)  = delete; \
    T& operator=(const T&&) = delete;

/**
 * A type decorator to make the type non-instantiable
 */
#define NON_INSTATIABLE(T)            \
    T()                     = delete; \
    T(const T&)             = delete; \
    T(const T&&)            = delete; \
    ~T()                    = delete; \
    T& operator=(const T&)  = delete; \
    T& operator=(const T&&) = delete;

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// UTILITIES /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#define SPUTNIK_ASSERT(x, ...)                                           \
    {                                                                    \
        if(!(x))                                                         \
        {                                                                \
            /*RENDERER_CRITICAL("Assertion failed: {0}", __VA_ARGS__);*/ \
            std::cout << "Assertion failed";                             \
            __debugbreak();                                              \
        }                                                                \
    }

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// DATA TYPES /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
