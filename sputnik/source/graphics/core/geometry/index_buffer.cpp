#include "pch.h"
#include "index_buffer.h"
#include "graphics/api/renderer.h"
#include "graphics/glcore/gl_index_buffer.h"

namespace sputnik::graphics::core
{

IndexBuffer::IndexBuffer() {}
IndexBuffer::~IndexBuffer() {}

std::shared_ptr<IndexBuffer> IndexBuffer::CreateIndexBuffer(ui32 const* indices, ui32 count)
{
    std::shared_ptr<IndexBuffer> buffer = nullptr;
    switch(api::Renderer::GetCurrentGraphicsSubsystemType())
    {
    case GraphicsSubsystemType::OPENGL:
        buffer = std::make_shared<glcore::GlIndexBuffer>(indices, count);
        break;
    default:
        break;
    }

    SPUTNIK_ASSERT(buffer != nullptr,
                   "Encounted an invalid graphics subsystem type: ",
                   api::Renderer::GetCurrentGraphicsSubsystemType());

    return buffer;
}

std::shared_ptr<IndexBuffer> IndexBuffer::CreateIndexBuffer(ui32 count)
{
    std::shared_ptr<IndexBuffer> buffer = nullptr;
    switch(api::Renderer::GetCurrentGraphicsSubsystemType())
    {
    case GraphicsSubsystemType::OPENGL:
        buffer = std::make_shared<glcore::GlIndexBuffer>(count);
        break;
    default:
        break;
    }

    SPUTNIK_ASSERT(buffer != nullptr,
                   "Encounted an invalid graphics subsystem type: ",
                   api::Renderer::GetCurrentGraphicsSubsystemType());

    return buffer;
}

} // namespace sputnik::graphics::core
