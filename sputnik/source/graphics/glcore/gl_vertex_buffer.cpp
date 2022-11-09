#include "pch.h"
#include "gl_vertex_buffer.h"

#include <glad/glad.h>

namespace sputnik::graphics::glcore
{

GlVertexBuffer::GlVertexBuffer(float* data, ui32 size) : m_size(size)
{
    // Creating an immutable buffer storage and initializing the data store.
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferStorage.xhtml
    glCreateBuffers(1, &m_renderer_id);
    glNamedBufferStorage(m_renderer_id, size, data, GL_DYNAMIC_STORAGE_BIT);
}

GlVertexBuffer::GlVertexBuffer(ui32 size) : m_size(size)
{
    // Creating an immutable buffer storage. Data store is uninitialized. A call to glNamedBufferSubData() should
    // be used to update the data store.
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferStorage.xhtml
    glCreateBuffers(1, &m_renderer_id);
    glNamedBufferStorage(m_renderer_id, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

GlVertexBuffer::~GlVertexBuffer()
{
    glDeleteBuffers(1, &m_renderer_id);
}

void GlVertexBuffer::Bind() const
{
    // Ideally this won't be required
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
}

void GlVertexBuffer::Unbind() const
{
    // Ideally this won't be required
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Todo:: Ideally we will also provide offset for the data
void GlVertexBuffer::UpdateData(void const* data, ui32 size) const
{
    SPUTNIK_ASSERT(size == m_size,
                   "Invalid data size provide while updating the index buffer. Max buffer size: {1}, passed size: {2}",
                   m_size,
                   size);

    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferSubData.xhtml
    glNamedBufferSubData(m_renderer_id, 0, size, data);
}

ui32 GlVertexBuffer::GetId() const
{
    return m_renderer_id;
}

} // namespace sputnik::graphics::glcore