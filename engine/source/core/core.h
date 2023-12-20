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

#define SPUTNIK_ASSERT(x, ...)                                    \
    {                                                             \
        if(!(x))                                                  \
        {                                                         \
            ENGINE_CRITICAL("Assertion failed: {}", __VA_ARGS__); \
            /*std::cout << "Assertion failed";*/                  \
            __debugbreak();                                       \
        }                                                         \
    }

#define SPUTNIK_ASSERT_MESSAGE(assertion, message, ...) \
    {                                                   \
        if(!(assertion))                                \
        {                                               \
            ENGINE_CRITICAL(message, __VA_ARGS__);      \
            /*std::cout << "Assertion failed";*/        \
            __debugbreak();                             \
        }                                               \
    }

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// DATA TYPES /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

using u8          = uint8_t;
using u16         = uint16_t;
using u32         = uint32_t;
using u64         = uint64_t;
using i8          = int8_t;
using i16         = int16_t;
using i32         = int32_t;
using i64         = int64_t;
using f32         = float;
using f64         = double;
using byte        = u8;
using cstring     = const char*;
const u64 u64_max = std::numeric_limits<u64>::max();
const u32 u32_max = std::numeric_limits<u32>::max();
const u16 u16_max = std::numeric_limits<u16>::max();
const u8  u8_max  = std::numeric_limits<u8>::max();
const i64 i64_max = std::numeric_limits<i64>::max();
const i32 i32_max = std::numeric_limits<i32>::max();
const i16 i16_max = std::numeric_limits<i16>::max();
const i8  i8_max  = std::numeric_limits<i8>::max();

///////////////////////////////////////////////////////////////////////////////////////////////////
