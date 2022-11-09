#pragma once

#include "graphics/core/geometry/gpu_data_type.h"

#include <glad/glad.h>

namespace sputnik::graphics::glcore
{
GLenum GetGlGpuTypeFromGpuType(core::GpuDataType type)
{
    GLenum gl_type = 0;
    switch(type)
    {
    case sputnik::graphics::core::GpuDataType::NONE:
        break;
    case sputnik::graphics::core::GpuDataType::FLOAT:
    case sputnik::graphics::core::GpuDataType::FLOAT2:
    case sputnik::graphics::core::GpuDataType::FLOAT3:
    case sputnik::graphics::core::GpuDataType::FLOAT4:
        gl_type = GL_FLOAT;
        break;
    case sputnik::graphics::core::GpuDataType::INT:
    case sputnik::graphics::core::GpuDataType::INT2:
    case sputnik::graphics::core::GpuDataType::INT3:
    case sputnik::graphics::core::GpuDataType::INT4:
        gl_type = GL_INT;
        break;

    case sputnik::graphics::core::GpuDataType::BOOL:
        gl_type = GL_BOOL;
        break;
    default:
        break;
    }

    SPUTNIK_ASSERT(gl_type != 0,
                   "Encountered invalid OpenGL type:",
                   type); // Todo: Check if the format of the log is correct

    return gl_type;
}

GLint GetGlGpuTypeSize(GLenum gl_type)
{
    GLint size = 0;
    switch(gl_type)
    {
    case sputnik::graphics::core::GpuDataType::NONE:
        break;
    case sputnik::graphics::core::GpuDataType::INT:
    case sputnik::graphics::core::GpuDataType::FLOAT:
        size = 4;
        break;
    case sputnik::graphics::core::GpuDataType::INT2:
    case sputnik::graphics::core::GpuDataType::FLOAT2:
        size = 8;
        break;
    case sputnik::graphics::core::GpuDataType::INT3:
    case sputnik::graphics::core::GpuDataType::FLOAT3:
        size = 12;
        break;
    case sputnik::graphics::core::GpuDataType::INT4:
    case sputnik::graphics::core::GpuDataType::FLOAT4:
        size = 16;
        break;
    case sputnik::graphics::core::GpuDataType::BOOL:
        size = 1;
        break;
    default:
        break;
    }
    return size;
}

} // namespace sputnik::graphics::glcore