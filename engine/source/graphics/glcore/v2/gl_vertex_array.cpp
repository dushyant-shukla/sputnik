#include "pch.h"
#include "gl_vertex_array.h"
#include "gl_buffer.h"
#include "core/core.h"

#include <glad/glad.h>

namespace sputnik::graphics::gl
{

cstring attributeTypeToString(const VertexAttributeType& type)
{
    switch(type)
    {
        SPUTNIK_CASE_TO_STRING(VertexAttributeType::Float);
        SPUTNIK_CASE_TO_STRING(VertexAttributeType::Float2);
        SPUTNIK_CASE_TO_STRING(VertexAttributeType::Float3);
        SPUTNIK_CASE_TO_STRING(VertexAttributeType::Float4);
        SPUTNIK_CASE_TO_STRING(VertexAttributeType::Mat3);
        SPUTNIK_CASE_TO_STRING(VertexAttributeType::Mat4);
        SPUTNIK_CASE_TO_STRING(VertexAttributeType::Int);
        SPUTNIK_CASE_TO_STRING(VertexAttributeType::Int2);
        SPUTNIK_CASE_TO_STRING(VertexAttributeType::Int3);
        SPUTNIK_CASE_TO_STRING(VertexAttributeType::Int4);
        SPUTNIK_CASE_TO_STRING(VertexAttributeType::Bool);
    }
    return "Unknown";
}

std::unordered_map<VertexAttributeType, u32> kAttributeTypeToSize = {{VertexAttributeType::Float, 4},
                                                                     {VertexAttributeType::Float2, 2 * 4},
                                                                     {VertexAttributeType::Float3, 3 * 4},
                                                                     {VertexAttributeType::Float4, 4 * 4},
                                                                     {VertexAttributeType::Int, 4},
                                                                     {VertexAttributeType::Int2, 2 * 4},
                                                                     {VertexAttributeType::Int3, 3 * 4},
                                                                     {VertexAttributeType::Int4, 4 * 4},
                                                                     {VertexAttributeType::Mat3, 3 * 3 * 4},
                                                                     {VertexAttributeType::Mat4, 4 * 4 * 4},
                                                                     {VertexAttributeType::Bool, 1}};

std::unordered_map<VertexAttributeType, u32> kAttributeTypeToComponentCount = {{VertexAttributeType::Float, 1},
                                                                               {VertexAttributeType::Float2, 2},
                                                                               {VertexAttributeType::Float3, 3},
                                                                               {VertexAttributeType::Float4, 4},
                                                                               {VertexAttributeType::Int, 1},
                                                                               {VertexAttributeType::Int2, 2},
                                                                               {VertexAttributeType::Int3, 3},
                                                                               {VertexAttributeType::Int4, 4},
                                                                               {VertexAttributeType::Mat3, 3 * 3},
                                                                               {VertexAttributeType::Mat4, 4 * 4},
                                                                               {VertexAttributeType::Bool, 1}};

std::unordered_map<VertexAttributeType, u32> kAttributeTypeToGlType = {{VertexAttributeType::Float, GL_FLOAT},
                                                                       {VertexAttributeType::Float2, GL_FLOAT},
                                                                       {VertexAttributeType::Float3, GL_FLOAT},
                                                                       {VertexAttributeType::Float4, GL_FLOAT},
                                                                       {VertexAttributeType::Int, GL_INT},
                                                                       {VertexAttributeType::Int2, GL_INT},
                                                                       {VertexAttributeType::Int3, GL_INT},
                                                                       {VertexAttributeType::Int4, GL_INT},
                                                                       {VertexAttributeType::Mat3, GL_FLOAT},
                                                                       {VertexAttributeType::Mat4, GL_FLOAT},
                                                                       {VertexAttributeType::Bool, GL_BOOL}};

inline u32 getAttributeSize(VertexAttributeType type)
{
    auto itr = kAttributeTypeToSize.find(type);
    if(itr == kAttributeTypeToSize.end())
    {
        SPUTNIK_ASSERT_MESSAGE(false, "Attribute Size: Unknown vertex attribute type: {}", static_cast<u32>(type));
    }
    return itr->second;
}

inline u32 getComponentCount(VertexAttributeType type)
{
    auto itr = kAttributeTypeToComponentCount.find(type);
    if(itr == kAttributeTypeToComponentCount.end())
    {
        SPUTNIK_ASSERT_MESSAGE(false,
                               "Attribute Component Count: Unknown vertex attribute type: {}",
                               static_cast<u32>(type));
    }
    return itr->second;
}

inline u32 getOglType(VertexAttributeType type)
{
    auto itr = kAttributeTypeToGlType.find(type);
    if(itr == kAttributeTypeToGlType.end())
    {
        SPUTNIK_ASSERT_MESSAGE(false, "Attribute Gl Type: Unknown vertex attribute type: {}", static_cast<u32>(type));
    }
    return itr->second;
}

OglVertexArray::OglVertexArray() : m_id{0}
{
    glGenVertexArrays(1, &m_id);
    SPUTNIK_ASSERT(m_id != 0, "Failed to create a vertex array.");
}

OglVertexArray::~OglVertexArray()
{
    glDeleteVertexArrays(1, &m_id);
}

void OglVertexArray::bind()
{
    glBindVertexArray(m_id);
}

void OglVertexArray::unbind()
{
    glBindVertexArray(0);
}

const u32& OglVertexArray::getId() const
{
    return m_id;
}

void OglVertexArray::addVertexBuffer(
    const OglBuffer&                                                buffer,
    const VertexInputBindingSpecification&                          binding_specification,
    const std::initializer_list<VertexInputAttributeSpecification>& attribute_specifications)
{
    // References:
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindVertexBuffer.xhtml (glVertexArrayVertexBuffer)
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glEnableVertexAttribArray.xhtml (glEnableVertexArrayAttrib)
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glVertexAttribFormat.xhtml (glVertexArrayAttribFormat)
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glVertexAttribBinding.xhtml (glVertexArrayAttribBinding)

    SPUTNIK_ASSERT_MESSAGE(binding_specification.binding_index < GL_MAX_VERTEX_ATTRIB_BINDINGS,
                           "Invalid vertex input binding specification. Provided vertex attribute binding index: {}. "
                           "The maximum number of vertex attribute bindings is {}.",
                           binding_specification.binding_index,
                           GL_MAX_VERTEX_ATTRIB_BINDINGS);

    bind();

    // Bind the buffer to the specified binding index.
    glVertexArrayVertexBuffer(m_id,
                              binding_specification.binding_index,
                              buffer.getId(),
                              0,
                              binding_specification.stride);

    u32 slot_index = 0;
    u32 offset     = 0;
    for(const auto& specification : attribute_specifications)
    {
        SPUTNIK_ASSERT_MESSAGE(
            slot_index < GL_MAX_VERTEX_ATTRIBS,
            "Invalid vertex input attribute specification. Provided vertex input attribute slot index: {}. "
            "The maximum number of vertex input attribute slots is {}.",
            slot_index,
            GL_MAX_VERTEX_ATTRIBS);

        glEnableVertexArrayAttrib(m_id, slot_index); // Enable the attribute stream at the specified slot index.

        // Specify the format of the attribute stream at the specified slot index.
        glVertexArrayAttribFormat(m_id,
                                  slot_index,
                                  getComponentCount(specification.type),
                                  getOglType(specification.type),
                                  specification.normalized ? GL_TRUE : GL_FALSE,
                                  offset);

        // Bind the attribute stream at the specified slot index to the specified binding index.
        glVertexArrayAttribBinding(m_id, slot_index, binding_specification.binding_index);

        offset += getAttributeSize(specification.type);
        ++slot_index;
    }

    unbind();
}

void OglVertexArray::addIndexBuffer(const OglBuffer& buffer)
{
    // Reference:
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glVertexArrayElementBuffer.xhtml

    glVertexArrayElementBuffer(m_id, buffer.getId());
}

} // namespace sputnik::graphics::gl