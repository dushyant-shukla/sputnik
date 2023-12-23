#include "pch.h"
#include "gl_context.h"
#include "core/core.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace sputnik::graphics::glcore
{

void oglMessageCallback(GLenum        source,
                        GLenum        type,
                        GLuint        id,
                        GLenum        severity,
                        GLsizei       length,
                        GLchar const* message,
                        void const*   user_param)
{
    auto const src_str = [source]()
    {
        switch(source)
        {
        case GL_DEBUG_SOURCE_API:
            return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "THIRD PARTY";
        case GL_DEBUG_SOURCE_APPLICATION:
            return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER:
            return "OTHER";
        }
    }();

    auto const type_str = [type]()
    {
        switch(type)
        {
        case GL_DEBUG_TYPE_ERROR:
            return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY:
            return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE:
            return "PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER:
            return "MARKER";
        case GL_DEBUG_TYPE_OTHER:
            return "OTHER";
        }
    }();

    auto const severity_str = [severity]()
    {
        switch(severity)
        {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return "NOTIFICATION";
        case GL_DEBUG_SEVERITY_LOW:
            return "LOW";
        case GL_DEBUG_SEVERITY_MEDIUM:
            return "MEDIUM";
        case GL_DEBUG_SEVERITY_HIGH:
            return "HIGH";
        }
    }();
    // std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';

    SPUTNIK_ASSERT_MESSAGE(false,
                           "OpenGL Error: Source: {}, Type: {}, Severity: {}, Id: {}, Message: {}",
                           src_str,
                           type_str,
                           severity_str,
                           id,
                           message);
}

GlContext::GlContext(GLFWwindow* window_handle) : m_window_handle(window_handle)
{
    glfwMakeContextCurrent(window_handle);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize Glad!";
    }
    // glfwSwapInterval(1); // Enable vsync: Limits the max frame rate to the screen refresh rate

    // #ifdef _DEBUG
    // Enable OpenGL debug messages
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(oglMessageCallback, nullptr);

    // Disable deprecated OpenGL functionality errors
    glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, GL_DONT_CARE, 0, nullptr, GL_FALSE);

    // Todo: Figure out the performance issue when running the rope bridge demo
    // Disable performance warnings
    glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, nullptr, GL_FALSE);

    // #endif // DEBUG
}

GlContext::~GlContext() {}

void GlContext::SwapBuffers()
{
    glfwSwapBuffers(m_window_handle);
}

} // namespace sputnik::graphics::glcore