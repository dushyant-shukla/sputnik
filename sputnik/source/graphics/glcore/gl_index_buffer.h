#pragma once

#include "core/core.h"
#include "graphics/core/geometry/index_buffer.h"

namespace sputnik::graphics::glcore
{

class GlIndexBuffer : public core::IndexBuffer
{

public:
    GlIndexBuffer(ui32 const* indices, ui32 count);
    GlIndexBuffer(ui32 count);
    virtual ~GlIndexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual ui32 GetCount() const override;
    virtual void UpdateData(ui32 const* indices, ui32 size) const override;
    ui32         GetId() const;

private:
    ui32 m_renderer_id;
    ui32 m_count;
};

} // namespace sputnik::graphics::glcore