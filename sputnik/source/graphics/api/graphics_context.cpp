#include "pch.h"
#include "graphics_context.h"
#include "graphics/glcore/opengl_context.h"

namespace sputnik::api
{

GraphicsContext::GraphicsContext(GLFWwindow* window_handle) : m_window_handle(window_handle) {}

GraphicsContext::~GraphicsContext() {}

GraphicsContext& GraphicsContext::InitializeGraphicsContext(GLFWwindow*              window_handle,
                                                            const GraphicsSubsystem& subsystem)
{
    SPUTNIK_ASSERT(subsystem != GraphicsSubsystem::NONE && subsystem != GraphicsSubsystem::VULKAN &&
                   subsystem != GraphicsSubsystem::DIRECTX);

    return sputnik::opengl::OpenglContext::InitializeOpenglContext(window_handle);
}

} // namespace sputnik::api