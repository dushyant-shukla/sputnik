#include "pch.h"
#include "gl_context.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace sputnik::graphics::glcore
{

GlContext::GlContext(GLFWwindow* window_handle) : m_window_handle(window_handle)
{
    glfwMakeContextCurrent(window_handle);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize Glad!";
    }
    glfwSwapInterval(1);
}

GlContext::~GlContext() {}

void GlContext::SwapBuffers()
{
    glfwSwapBuffers(m_window_handle);
}

} // namespace sputnik::graphics::glcore