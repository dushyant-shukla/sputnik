#pragma once

#include "core/core.h"

class GLFWwindow;

namespace sputnik::api
{

enum class GraphicsSubsystem
{
    NONE,
    OPENGL,
    VULKAN,
    DIRECTX
};

class GraphicsContext
{

public:
    NON_COPYABLE(GraphicsContext);

    virtual ~GraphicsContext();
    static GraphicsContext& InitializeGraphicsContext(GLFWwindow* window_handle, const GraphicsSubsystem& subsystem);
    virtual void            SwapBuffers() = 0;

protected:
    GraphicsContext(GLFWwindow* window_handle);
    GLFWwindow* m_window_handle;
};

} // namespace sputnik::api