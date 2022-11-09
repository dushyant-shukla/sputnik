#include "pch.h"
#include "gl_vertex_array.h"
#include "graphics/core/geometry/vertex_attribute_data_type.h"

#include <glad/glad.h>

namespace sputnik::graphics::glcore
{

GlVertexArray::GlVertexArray()
{
    SPUTNIK_ASSERT(false, "Not implemented!");
    //glCreateVertexArrays(1, &m_renderer_id);
}

GlVertexArray::GlVertexArray(const core::VertexArrayLayout& layout)
{
    glCreateVertexArrays(1, &m_renderer_id);
}

GlVertexArray::~GlVertexArray()
{
    glDeleteVertexArrays(1, &m_renderer_id);
}

void GlVertexArray::Bind() const
{
    glBindVertexArray(m_renderer_id);
}

void GlVertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void GlVertexArray::AddVertexBuffer(std::shared_ptr<graphics::core::VertexBuffer>& vertex_buffer)
{
    // With DSA, no need to bind the vertex array
    m_vertex_buffers.emplace_back(vertex_buffer);

    ui32 attribute_stream_index = 0;
    const auto& vb_layout = vertex_buffer->GetBufferLayout();
    for(const auto& attribute: vb_layout)
    {
        switch(attribute.m_type)
        {
        case core::GpuDataType::INT:
        case core::GpuDataType::INT2:
        case core::GpuDataType::INT3:
        case core::GpuDataType::INT4:
        case core::GpuDataType::BOOL:
        {
            glEnableVertexArrayAttrib(m_renderer_id, attribute_stream_index++); // enable the vertex attribute stream
            glVertexArrayAttribFormat(m_renderer_id, attribute_stream_index, // component count of the attribute, // data type of the attribute, // normalize?, // offset);
        }
        default:
            break;
        }
    }
}

void GlVertexArray::SetIndexBuffer(std::shared_ptr<graphics::core::IndexBuffer>& index_buffer)
{
    // With DSA we do not need to bind the VAO before binding the index buffer to the VAO
    m_index_buffer = index_buffer;
    glVertexArrayElementBuffer(m_renderer_id, m_index_buffer->GetId());
}

} // namespace sputnik::graphics::glcore