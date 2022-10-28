#pragma once

#include "graphics/api/graphics_context.h"

class GLFWwindow;

namespace sputnik::opengl
{

class OpenglContext : public api::GraphicsContext
{

public:
    NON_COPYABLE(OpenglContext);

    virtual ~OpenglContext();
    static OpenglContext& InitializeOpenglContext(GLFWwindow* window_handle);
    virtual void SwapBuffers() override;

protected:
    OpenglContext(GLFWwindow* window_handle);
};

} // namespace sputnik::opengl