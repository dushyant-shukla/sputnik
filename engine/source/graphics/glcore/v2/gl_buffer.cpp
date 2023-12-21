#include "pch.h"

#include "gl_buffer.h"

#include <glad/glad.h>

namespace sputnik::graphics::gl
{

// More information at:
// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferStorage.xhtml
// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferSubData.xhtml
// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBufferBase.xhtml
// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBufferRange.xhtml

const u32 BufferUsageFlagBits::kDynamicStorageBit = GL_DYNAMIC_STORAGE_BIT;
const u32 BufferUsageFlagBits::kMapReadBit        = GL_MAP_READ_BIT;
const u32 BufferUsageFlagBits::kMapWriteBit       = GL_MAP_WRITE_BIT;
const u32 BufferUsageFlagBits::kMapPersistentBit  = GL_MAP_PERSISTENT_BIT;
const u32 BufferUsageFlagBits::kMapCoherentBit    = GL_MAP_COHERENT_BIT;
const u32 BufferUsageFlagBits::kClientStorageBit  = GL_CLIENT_STORAGE_BIT;

OglBuffer::OglBuffer(void* data, const u64& bytes) : m_id(0), m_bytes{bytes}, m_data{data}
{
    glCreateBuffers(1, &m_id);
    SPUTNIK_ASSERT(m_id != 0, "Failed to create a buffer.");
    // Initializes and creates buffer's immutable data store, i.e. the buffer's data store cannot be
    // reinitialized/resized. We can however, send new data to the GPU. It is an error (GL_INVALID_OPERATION) to call
    // this function on a buffer that has already been allocated with this function before.
    //
    // GL_DYNAMIC_STORAGE_BIT: The buffer's data store contents will be modified repeatedly through calls to
    // glBufferSubData().
    //
    // The alternative to this function is glBufferData()/glNamedBufferData(), which is the same as glBufferStorage()
    // except that it can be called multiple times for the same buffer, and it will orphan the old memory and allocate
    // new storage.
    glNamedBufferStorage(m_id, bytes, data, GL_DYNAMIC_STORAGE_BIT);
}

OglBuffer::OglBuffer(void* data, const u64& bytes, const u32& usage_flags) : m_id{0}, m_bytes{bytes}, m_data{data}
{
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, bytes, data, usage_flags);
}

OglBuffer::OglBuffer(const u64& bytes) : m_id(0), m_bytes{bytes}, m_data{nullptr}
{
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, bytes, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

OglBuffer::OglBuffer(const u64& bytes, const u32& usage_flags) : m_id{0}, m_bytes{bytes}, m_data{nullptr}
{
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, bytes, nullptr, usage_flags);
}

OglBuffer::~OglBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void OglBuffer::setData(void* data, u64 bytes)
{
    m_bytes = bytes;
    m_data  = data;
    glNamedBufferSubData(m_id, 0, bytes, data);
}

const u32& OglBuffer::getId() const
{
    return m_id;
}

void OglBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void OglBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace sputnik::graphics::gl
