#pragma once

#include "core/logging/logging_core.h"

#include <stdint.h>

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
            ENGINE_CRITICAL("Assertion has failed: {0}", __VA_ARGS__);   \
            __debugbreak();                                              \
        }                                                                \
    }

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// DATA TYPES /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

typedef uint8_t  ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;
typedef uint64_t ui64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
