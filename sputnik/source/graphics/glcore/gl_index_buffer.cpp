#include "pch.h"
#include "gl_index_buffer.h"

#include <glad/glad.h>

namespace sputnik::graphics::glcore
{

GlIndexBuffer::GlIndexBuffer(ui32 const* indices, ui32 count) : m_count(count)
{
    // Creating an immutable buffer storage and initializing the data store.
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferStorage.xhtml
    glCreateBuffers(1, &m_renderer_id);
    glNamedBufferStorage(m_renderer_id, sizeof(ui32) * m_count, indices, GL_DYNAMIC_STORAGE_BIT);
}

GlIndexBuffer::GlIndexBuffer(ui32 count) : m_count(count)
{
    // Creating an immutable buffer storage. Data store is uninitialized. A call to glNamedBufferSubData() should
    // be used to update the data store.
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferStorage.xhtml
    glCreateBuffers(1, &m_renderer_id);
    glNamedBufferStorage(m_renderer_id, sizeof(ui32) * m_count, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

GlIndexBuffer::~GlIndexBuffer()
{
    glDeleteBuffers(1, &m_renderer_id);
}

void GlIndexBuffer::Bind() const
{
    // Ideally this won't be required
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
}

void GlIndexBuffer::Unbind() const
{
    // Ideally this won't be required
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

ui32 GlIndexBuffer::GetCount() const
{
    return m_count;
}

void GlIndexBuffer::UpdateData(ui32 const* indices, ui32 size) const
{
    ui32 max_size = m_count * sizeof(ui32);
    SPUTNIK_ASSERT(size == max_size,
                   "Invalid data size provide while updating the index buffer. Max buffer size: {1}, passed size: {2}",
                   max_size,
                   size);

    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferSubData.xhtml
    glNamedBufferSubData(m_renderer_id, 0, size, indices);
}

ui32 GlIndexBuffer::GetId() const
{
    return m_renderer_id;
}

} // namespace sputnik::graphics::glcore