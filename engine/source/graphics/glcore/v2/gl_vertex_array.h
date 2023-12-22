#pragma once

#include "core/core.h"

#include <vector.hpp>

namespace sputnik::graphics::gl
{

using namespace ramanujan::experimental;

class OglBuffer;

enum class VertexAttributeType : u32
{
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

cstring attributeTypeToString(const VertexAttributeType& type);

struct Vertex
{
    vec3 position;
    vec3 normal;
    vec2 uv;

    //   static VertexInputBindingSpecification getBindingSpecification() { return {0, sizeof(Vertex)}; }

    //   static std::vector<VertexInputAttributeSpecification> getAttributeSpecifications()
    //{
    //	return {{0, 0, offsetof(Vertex, position), sizeof(vec3), GL_FLOAT, GL_FALSE},
    //			{0, 1, offsetof(Vertex, normal), sizeof(vec3), GL_FLOAT, GL_FALSE},
    //			{0, 2, offsetof(Vertex, uv), sizeof(vec2), GL_FLOAT, GL_FALSE}};
    //}
};

struct VertexInputBindingSpecification
{
    u32 binding_index;
    u32 stride;
};

struct VertexInputAttributeSpecification
{
    std::string         name;
    u32                 location;
    VertexAttributeType type;
    bool                normalized;
};

class OglVertexArray
{
public:
    OglVertexArray();
    ~OglVertexArray();

    OglVertexArray(OglVertexArray&&)            = default;
    OglVertexArray& operator=(OglVertexArray&&) = default;

    void bind() const;
    void unbind() const;

    const u32& getId() const;

    void addVertexBuffer(const OglBuffer&                                                buffer,
                         const VertexInputBindingSpecification&                          buffer_specification,
                         const std::initializer_list<VertexInputAttributeSpecification>& attribute_specifications);

    void addIndexBuffer(const OglBuffer& buffer);

private:
    OglVertexArray(const OglVertexArray&)            = delete;
    OglVertexArray& operator=(const OglVertexArray&) = delete;

private:
    u32 m_id;
};

} // namespace sputnik::graphics::gl