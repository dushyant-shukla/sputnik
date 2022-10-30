#pragma once

#include "core/core.h"

class GLFWwindow;

namespace sputnik::graphics::core
{

// Todo: This class is not needed
class GraphicsContext
{

public:
    virtual ~GraphicsContext();
    virtual void SwapBuffers() = 0;

protected:
    GraphicsContext(GLFWwindow* window_handle);

protected:
    GLFWwindow* m_window_handle;
};

} // namespace sputnik::graphics::core