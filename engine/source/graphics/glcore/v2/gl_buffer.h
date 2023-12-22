#pragma once

#include "core/core.h"

namespace sputnik::graphics::gl
{

class BufferUsageFlagBits
{
public:
    NON_INSTATIABLE(BufferUsageFlagBits)

    static const u32 kDynamicStorageBit;
    static const u32 kMapReadBit;
    static const u32 kMapWriteBit;
    static const u32 kMapPersistentBit;
    static const u32 kMapCoherentBit;
    static const u32 kClientStorageBit;
};

enum class BufferBindTarget : u32
{
    Invalid = 0,
    VertexBuffer,
    IndexBuffer,
    UniformBuffer,
    ShaderStorageBuffer,
    TextureBuffer
};

class OglBuffer
{

public:
    /*!
     * @brief .
     */
    OglBuffer();

    /*!
     * @brief .
     *
     * @param bytes
     */
    OglBuffer(const u64& bytes);

    /*!
     * @brief .
     *
     * @param data
     * @param bytes
     */
    OglBuffer(void* data, const u64& bytes);

    /*!
     * @brief .
     *
     * @param data
     * @param bytes
     * @param usage_flags
     */
    OglBuffer(void* data, const u64& bytes, const u32& usage_flags);

    /*!
     * @brief .
     *
     * @param bytes
     * @param usage_flags
     */
    OglBuffer(const u64& bytes, const u32& usage_flags);

    /*!
     * @brief .
     *
     */
    ~OglBuffer();

    OglBuffer(OglBuffer&& other) noexcept;

    OglBuffer& operator=(OglBuffer&& other) noexcept;

    /*!
     * @brief .
     *
     * @param data
     * @param bytes
     */
    void setData(void* data, u64 bytes);

    const u32& getId() const;

    void bind(const BufferBindTarget& bind_target);
    void bind(const BufferBindTarget& bind_target, const u32& bind_index);
    void bind(const BufferBindTarget& bind_target, const u32& bind_index, void* offset, void* size);
    void unbind();

private:
    OglBuffer(const OglBuffer&)            = delete;
    OglBuffer& operator=(const OglBuffer&) = delete;

private:
    u32              m_id;
    BufferBindTarget m_bind_target;

    void* m_data;
    u64   m_bytes;
};

} // namespace sputnik::graphics::gl
