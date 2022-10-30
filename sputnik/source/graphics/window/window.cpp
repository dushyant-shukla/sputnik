#include "pch.h"
#include "window.h"
#include "graphics/glcore/gl_context.h"
#include "main/application.h"
#include "graphics/api/renderer.h"

#include <GLFW/glfw3.h>

namespace sputnik::graphics::window
{

static void GLFWErrorCallback(int error, const char* description)
{
    std::cout << "GLFW Error " << error << " : " << description;
}

Window::Window(const WindowSpecification& specification)
{
    if(!glfwInit())
    {
        std::cout << "Failed to intialize GLFW.";
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetErrorCallback(GLFWErrorCallback);

    m_window_handle = glfwCreateWindow((int)specification.m_width,
                                       (int)specification.m_height,
                                       specification.m_title.c_str(),
                                       nullptr,
                                       nullptr);

    glfwSetWindowUserPointer(m_window_handle, &m_window_specification);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(m_window_handle,
                              [](GLFWwindow* window, int width, int height)
                              {
                                  WindowSpecification& window_specification =
                                      *(WindowSpecification*)glfwGetWindowUserPointer(window);
                                  window_specification.m_width  = width;
                                  window_specification.m_height = height;
                                  glfwRequestWindowAttention(window);
                                  // Todo:: This should happen with events (this is only temporary)
                                  api::Renderer::OnWindowResize(width, height);
                              });

    glfwSetWindowCloseCallback(m_window_handle,
                               [](GLFWwindow* window)
                               {
                                   WindowSpecification& window_specification =
                                       *(WindowSpecification*)glfwGetWindowUserPointer(window);

                                   // Todo:: Temporary (This must be triggered through an event)
                                   main::Application::GetInstance()->Shutdown();
                               });

    glfwSetKeyCallback(m_window_handle,
                       [](GLFWwindow* window, int key, int scanCode, int action, int mods)
                       {
                           WindowSpecification& window_specification =
                               *(WindowSpecification*)glfwGetWindowUserPointer(window);
                           // glfwMaximizeWindow(window);
                           switch(action)
                           {
                           case GLFW_PRESS:
                           {
                               break;
                           }

                           case GLFW_RELEASE:
                           {
                               break;
                           }

                           case GLFW_REPEAT:
                           {
                               break;
                           }
                           }
                       });

    glfwSetCharCallback(m_window_handle,
                        [](GLFWwindow* window, unsigned int keyCode) {
                            WindowSpecification& window_specification =
                                *(WindowSpecification*)glfwGetWindowUserPointer(window);
                        });

    glfwSetMouseButtonCallback(m_window_handle,
                               [](GLFWwindow* window, int button, int action, int mods)
                               {
                                   WindowSpecification& window_specification =
                                       *(WindowSpecification*)glfwGetWindowUserPointer(window);
                                   switch(action)
                                   {
                                   case GLFW_PRESS:
                                   {
                                       break;
                                   }

                                   case GLFW_RELEASE:
                                   {
                                       break;
                                   }
                                   }
                               });

    glfwSetScrollCallback(m_window_handle,
                          [](GLFWwindow* window, double offsetX, double offsetY) {
                              WindowSpecification& window_specification =
                                  *(WindowSpecification*)glfwGetWindowUserPointer(window);
                          });

    glfwSetCursorPosCallback(m_window_handle,
                             [](GLFWwindow* window, double positionX, double positionY) {
                                 WindowSpecification& window_specification =
                                     *(WindowSpecification*)glfwGetWindowUserPointer(window);
                             });
}

Window::~Window()
{
    Shutdown();
}

// Window& Window::InitializeWindow(const WindowSpecification& specification)
//{
//     static Window instance(specification);
//     return instance;
// }

unsigned int Window::GetWidth() const
{
    return m_window_specification.m_width;
}

unsigned int Window::GetHeight() const
{
    return m_window_specification.m_height;
}

void Window::SetVsync(bool enable_vsync)
{
    if(enable_vsync)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }
    m_window_specification.m_vsync_enabled = enable_vsync;
}

bool Window::IsVsyncEnabled()
{
    return m_window_specification.m_vsync_enabled;
}

GLFWwindow* Window::GetNativeWindow()
{
    return m_window_handle;
}

void Window::OnUpdate()
{
    glfwPollEvents();
}

void Window::Shutdown()
{
    glfwDestroyWindow(m_window_handle);
}

} // namespace sputnik::graphics::window