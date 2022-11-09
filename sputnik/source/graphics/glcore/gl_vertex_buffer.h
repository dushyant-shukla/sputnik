#pragma once

#include "graphics/core/geometry/vertex_buffer.h"

namespace sputnik::graphics::glcore
{

class GlVertexBuffer : public core::VertexBuffer
{

public:
    GlVertexBuffer(float* data, ui32 size);
    GlVertexBuffer(ui32 size);
    virtual ~GlVertexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void UpdateData(void const* data, ui32 size) const override;
    ui32         GetId() const;

private:
    ui32 m_renderer_id;
    ui32 m_size;
};

} // namespace sputnik::graphics::glcore