#include "pch.h"
#include "vertex_buffer.h"
#include "graphics/api/renderer.h"
#include "graphics/glcore/gl_vertex_buffer.h"

namespace sputnik::graphics::core
{

VertexBuffer::~VertexBuffer() {}

std::shared_ptr<VertexBuffer> VertexBuffer::CreateBuffer(ui32 size)
{
    std::shared_ptr<VertexBuffer> buffer = nullptr;
    switch(api::Renderer::GetCurrentGraphicsSubsystemType())
    {
    case GraphicsSubsystemType::OPENGL:
        buffer = std::make_shared<glcore::GlVertexBuffer>(size);
        break;
    default:
        break;
    }
    return nullptr;
}

std::shared_ptr<VertexBuffer> VertexBuffer::CreateBuffer(float* data, ui32 size)
{
    std::shared_ptr<VertexBuffer> buffer = nullptr;
    switch(api::Renderer::GetCurrentGraphicsSubsystemType())
    {
    case GraphicsSubsystemType::OPENGL:
        buffer = std::make_shared<glcore::GlVertexBuffer>(data, size);
        break;
    default:
        break;
    }
    return nullptr;
}

} // namespace sputnik::graphics::core