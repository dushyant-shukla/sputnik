#pragma once

#include "core/core.h"

namespace sputnik::graphics::core
{

enum class VertexAttributeDataType
{
    NONE = 0,
    FLOAT,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    INT,
    INT2,
    INT3,
    INT4,
    BOOL
};

ui32 GetAttributeDataTypeSize(VertexAttributeDataType type)
{
    uint32_t size = 0;
    switch(type)
    {
    case VertexAttributeDataType::NONE:
        break;
    case VertexAttributeDataType::INT:
    case VertexAttributeDataType::FLOAT:
        size = 4;
        break;
    case VertexAttributeDataType::INT2:
    case VertexAttributeDataType::FLOAT2:
        size = 8;
        break;
    case VertexAttributeDataType::INT3:
    case VertexAttributeDataType::FLOAT3:
        size = 12;
        break;
    case VertexAttributeDataType::INT4:
    case VertexAttributeDataType::FLOAT4:
        size = 16;
        break;
    case VertexAttributeDataType::BOOL:
        size = 1;
        break;
    default:
        break;
    }

    SPUTNIK_ASSERT(size != 0, "Encountered invalid attribute data type: ", type);

    return size;
}

ui32 GetAttributeDataTypeComponentCount(VertexAttributeDataType type)
{
    ui32 count = 0;
    switch(type)
    {
    case VertexAttributeDataType::NONE:
        break;
    case VertexAttributeDataType::INT:
    case VertexAttributeDataType::FLOAT:
    case VertexAttributeDataType::BOOL:
        count = 1;
        break;
    case VertexAttributeDataType::INT2:
    case VertexAttributeDataType::FLOAT2:
        count = 2;
        break;
    case VertexAttributeDataType::INT3:
    case VertexAttributeDataType::FLOAT3:
        count = 3;
        break;
    case VertexAttributeDataType::INT4:
    case VertexAttributeDataType::FLOAT4:
        count = 4;
        break;
    default:
        break;
    }

    SPUTNIK_ASSERT(count != 0, "Encountered invalid attribute data type: ", type);

    return count;
}

} // namespace sputnik::graphics::core