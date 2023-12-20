#include "pch.h"

#include "gl_buffer.h"

#include <glad/glad.h>

namespace sputnik::graphics::glcore::v2
{

// More information at https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferStorage.xhtml

const u32 BufferUsageFlags::kDynamicStorageBit = GL_DYNAMIC_STORAGE_BIT;
const u32 BufferUsageFlags::kMapReadBit        = GL_MAP_READ_BIT;
const u32 BufferUsageFlags::kMapWriteBit       = GL_MAP_WRITE_BIT;
const u32 BufferUsageFlags::kMapPersistentBit  = GL_MAP_PERSISTENT_BIT;
const u32 BufferUsageFlags::kMapCoherentBit    = GL_MAP_COHERENT_BIT;
const u32 BufferUsageFlags::kClientStorageBit  = GL_CLIENT_STORAGE_BIT;

GlBuffer::GlBuffer(void* data, const u64& bytes) : m_id(0), m_bytes{bytes}, m_data{data}
{
    glCreateBuffers(1, &m_id);

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

GlBuffer::GlBuffer(void* data, const u64& bytes, const u32& usage_flags) : m_id{0}, m_bytes{bytes}, m_data{data}
{
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, bytes, data, usage_flags);
}

GlBuffer::GlBuffer(const u64& bytes) : m_id(0), m_bytes{bytes}, m_data{nullptr}
{
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, bytes, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

GlBuffer::GlBuffer(const u64& bytes, const u32& usage_flags) : m_id{0}, m_bytes{bytes}, m_data{nullptr}
{
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, bytes, nullptr, usage_flags);
}

GlBuffer::~GlBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void GlBuffer::setData(void* data, u64 bytes)
{
    m_bytes = bytes;
    m_data  = data;
    glNamedBufferSubData(m_id, 0, bytes, data);
}

} // namespace sputnik::graphics::glcore::v2
