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

class OglBuffer
{

public:
    NON_COPYABLE(OglBuffer)

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
     */
    OglBuffer(const u64& bytes);

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

    /*!
     * @brief .
     *
     * @param data
     * @param bytes
     */
    void setData(void* data, u64 bytes);

    const u32& getId() const;

    void bind() const;
    void unbind() const;

private:
    u32 m_id;

    void* m_data;
    u64   m_bytes;
};

} // namespace sputnik::graphics::gl
