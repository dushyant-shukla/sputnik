#pragma once

struct GLFWwindow;

namespace sputnik::graphics::glcore
{

class GlContext
{

public:
    GlContext(GLFWwindow* window_handle);
    virtual ~GlContext();
    virtual void SwapBuffers();

private:
    GLFWwindow* m_window_handle;
};

} // namespace sputnik::graphics::glcore