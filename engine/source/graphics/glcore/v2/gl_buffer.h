#pragma once

#include "core/core.h"

namespace sputnik::graphics::glcore::v2
{

class BufferUsageFlags
{
public:
    NON_INSTATIABLE(BufferUsageFlags)

    static const u32 kDynamicStorageBit;
    static const u32 kMapReadBit;
    static const u32 kMapWriteBit;
    static const u32 kMapPersistentBit;
    static const u32 kMapCoherentBit;
    static const u32 kClientStorageBit;
};

class GlBuffer
{

public:
    NON_COPYABLE(GlBuffer)

    /*!
     * @brief .
     *
     * @param data
     * @param bytes
     */
    GlBuffer(void* data, const u64& bytes);

    /*!
     * @brief .
     *
     * @param data
     * @param bytes
     * @param usage_flags
     */
    GlBuffer(void* data, const u64& bytes, const u32& usage_flags);

    /*!
     * @brief .
     *
     * @param bytes
     */
    GlBuffer(const u64& bytes);

    /*!
     * @brief .
     *
     * @param bytes
     * @param usage_flags
     */
    GlBuffer(const u64& bytes, const u32& usage_flags);

    /*!
     * @brief .
     *
     */
    ~GlBuffer();

    /*!
     * @brief .
     *
     * @param data
     * @param bytes
     */
    void setData(void* data, u64 bytes);

private:
    u32 m_id;

    void* m_data;
    u64   m_bytes;
};

} // namespace sputnik::graphics::glcore::v2
