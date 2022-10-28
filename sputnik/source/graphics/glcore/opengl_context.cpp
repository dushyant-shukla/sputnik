#include "pch.h"
#include "opengl_context.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace sputnik::opengl
{

OpenglContext::OpenglContext(GLFWwindow* window_handle) : GraphicsContext(window_handle)
{
    glfwMakeContextCurrent(window_handle);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize Glad!";
    }
}

OpenglContext::~OpenglContext() {}

void OpenglContext::SwapBuffers()
{
    glfwSwapBuffers(m_window_handle);
}

} // namespace sputnik::sputnik