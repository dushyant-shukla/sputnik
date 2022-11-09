#pragma once

#include "core/core.h"
#include "graphics/core/geometry/vertex_array.h"
#include "graphics/core/geometry/vertex_buffer.h"
#include "graphics/core/geometry/index_buffer.h"

namespace sputnik::graphics::glcore
{

class GlVertexArray : public graphics::core::VertexArray
{

public:
    GlVertexArray();
    GlVertexArray(const core::VertexArrayLayout& layout);
    virtual ~GlVertexArray();

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void AddVertexBuffer(std::shared_ptr<graphics::core::VertexBuffer>& vertex_buffer) override;
    // void AddVertexBuffer(void* vbid, int slot = 0);
    virtual const std::vector<std::shared_ptr<graphics::core::VertexBuffer>>& GetVertexBuffers() const override;
    virtual void SetIndexBuffer(std::shared_ptr<graphics::core::IndexBuffer>& index_buffer) override;
    virtual const std::shared_ptr<graphics::core::IndexBuffer>& GetIndexBuffer() const override;

protected:
    // protected methods

private:
    // private methods

private:
    std::vector<std::shared_ptr<graphics::core::VertexBuffer>> m_vertex_buffers;
    std::shared_ptr<graphics::core::IndexBuffer>               m_index_buffer;
    ui32                                                       m_renderer_id;
};

} // namespace sputnik::graphics::glcore

// Instead of having separate classes for index and vertex buffers, provide the methods in those classes to vertex array
// class. Let VA class handle creation/updation of buffers. It would be simpler implementation as well.
