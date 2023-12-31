#include "pch.h"

#include "gl_buffer.h"

#include <glad/glad.h>

namespace sputnik::graphics::gl
{

// Useful references:
// https://www.khronos.org/opengl/wiki/Buffer_Object
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

u32 getOglBufferBindTarget(const BufferBindTarget& bind_target)
{
    switch(bind_target)
    {
    case BufferBindTarget::VertexBuffer:
        return GL_ARRAY_BUFFER;
    case BufferBindTarget::IndexBuffer:
        return GL_ELEMENT_ARRAY_BUFFER;
    case BufferBindTarget::UniformBuffer:
        return GL_UNIFORM_BUFFER;
    case BufferBindTarget::ShaderStorageBuffer:
        return GL_SHADER_STORAGE_BUFFER;
    case BufferBindTarget::TextureBuffer:
        return GL_TEXTURE_BUFFER;
    default:
        SPUTNIK_ASSERT(false, "Invalid buffer bind target.");
        return 0;
    }
}

OglBuffer::OglBuffer() : m_id(0), m_bind_target{0}, m_bytes{0}, m_data{nullptr}
{
    glCreateBuffers(1, &m_id);
    SPUTNIK_ASSERT(m_id != 0, "Failed to create a buffer.");
}

OglBuffer::OglBuffer(void* data, const u64& bytes) : m_id(0), m_bind_target{0}, m_bytes{bytes}, m_data{data}
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

OglBuffer::OglBuffer(void* data, const u64& bytes, const u32& usage_flags)
    : m_id{0}
    , m_bind_target{0}
    , m_bytes{bytes}
    , m_data{data}
{
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, bytes, data, usage_flags);
}

OglBuffer::OglBuffer(const u64& bytes) : m_id(0), m_bind_target{0}, m_bytes{bytes}, m_data{nullptr}
{
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, bytes, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

OglBuffer::OglBuffer(const u64& bytes, const u32& usage_flags)
    : m_id{0}
    , m_bind_target{0}
    , m_bytes{bytes}
    , m_data{nullptr}
{
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, bytes, nullptr, usage_flags);
}

OglBuffer::~OglBuffer()
{
    SPUTNIK_ASSERT(m_id != 0, "Buffer is not initialized.");
    glDeleteBuffers(1, &m_id);
}

OglBuffer::OglBuffer(OglBuffer&& other) noexcept
{
    *this = std::move(other);
}

OglBuffer& OglBuffer::operator=(OglBuffer&& other) noexcept
{
    // TODO:: Need to implement this
    if(this != &other)
    {
        m_id          = other.m_id;
        m_bind_target = other.m_bind_target;
        m_bytes       = other.m_bytes;
        m_data        = other.m_data;
        other.m_id    = 0;
    }
    return *this;
}

void OglBuffer::setSize(const u64& bytes)
{
    SPUTNIK_ASSERT(m_id != 0, "Buffer is not initialized.");
    m_bytes = bytes;
    glNamedBufferStorage(m_id, bytes, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

void OglBuffer::setData(void* data, u64 bytes)
{
    SPUTNIK_ASSERT(m_id != 0, "Buffer is not initialized.");
    if(m_bytes == 0)
    {
        glNamedBufferStorage(m_id, bytes, data, GL_DYNAMIC_STORAGE_BIT);
    }
    m_bytes = bytes;
    m_data  = data;
    glNamedBufferSubData(m_id, 0, bytes, data);
}

const u32& OglBuffer::getId() const
{
    return m_id;
}

void OglBuffer::bind(const BufferBindTarget& bind_target)
{
    // Reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
    SPUTNIK_ASSERT(m_id != 0, "Buffer is not initialized.");
    m_bind_target = bind_target;
    glBindBuffer(getOglBufferBindTarget(m_bind_target), m_id);
}

void OglBuffer::bind(const BufferBindTarget& bind_target, const u32& bind_index)
{
    // Reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBufferBase.xhtml
    SPUTNIK_ASSERT(m_id != 0, "Buffer is not initialized.");
    m_bind_target = bind_target;
    glBindBufferBase(getOglBufferBindTarget(bind_target), bind_index, m_id);
}

void OglBuffer::bind(const BufferBindTarget& bind_target, const u32& bind_index, void* offset, void* size)
{
    // Reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBufferRange.xhtml
    SPUTNIK_ASSERT(m_id != 0, "Buffer is not initialized.");
    m_bind_target = bind_target;
    glBindBufferRange(getOglBufferBindTarget(bind_target), bind_index, m_id, (GLintptr)offset, (GLsizeiptr)size);
}

void OglBuffer::unbind()
{
    glBindBuffer(getOglBufferBindTarget(m_bind_target), 0);
    m_bind_target = BufferBindTarget::Invalid;
}

} // namespace sputnik::graphics::gl
