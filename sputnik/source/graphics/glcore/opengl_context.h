#pragma once

#include "graphics/api/graphics_context.h"

class GLFWwindow;

namespace sputnik::opengl
{

class OpenglContext : public api::GraphicsContext
{

public:
    OpenglContext(GLFWwindow* window_handle);
    virtual ~OpenglContext();
    virtual void SwapBuffers() override;

protected:
};

} // namespace sputnik::opengl