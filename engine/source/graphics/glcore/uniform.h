#pragma once

#include "pch.h"
#include "core/core.h"

namespace sputnik::graphics::glcore
{

template <typename T>
class Uniform
{

public:
    NON_INSTATIABLE(Uniform);

    static void Set(unsigned int slot, const T& value);
    static void Set(unsigned int slot, T* values, unsigned int count);
    static void Set(unsigned int slot, std::vector<T>& values);
};

} // namespace sputnik::graphics::glcore