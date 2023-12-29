#include "pch.h"
#include "window.h"
#include "main/application.h"
#include "editor/editor_camera.h"
#include "graphics/api/camera.h"
#include "core/systems/render_system.h"

#include <GLFW/glfw3.h>

#include <stb_image.h>

namespace sputnik::graphics::window
{

static void GLFWErrorCallback(int error, const char* description)
{
    std::cout << "GLFW Error " << error << " : " << description;
}

Window::Window(const WindowSpecification& specification)
{
    // Useful resources: https://www.glfw.org/docs/3.3/window_guide.html
    // https://www.glfw.org/docs/3.3/monitor_guide.html
    // https://www.glfw.org/docs/3.0/monitor.html

    if(!glfwInit())
    {
        std::cout << "Failed to intialize GLFW.";
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetErrorCallback(GLFWErrorCallback);

    // windowed fullscreen window
    {
        m_window_handle = glfwCreateWindow((int)specification.m_width,
                                           (int)specification.m_height,
                                           specification.m_title.c_str(),
                                           nullptr,
                                           nullptr);
        glfwMaximizeWindow(m_window_handle);
        int w, h;
        glfwGetFramebufferSize(m_window_handle, &w, &h);
        m_window_specification.m_height = h;
        m_window_specification.m_width  = w;
        glfwSetWindowUserPointer(m_window_handle, &m_window_specification);
    }

    //// fullscreen
    //{
    //    GLFWmonitor*       monitor = glfwGetPrimaryMonitor();
    //    const GLFWvidmode* mode    = glfwGetVideoMode(monitor);
    //    m_window_handle = glfwCreateWindow(mode->width, mode->height, specification.m_title.c_str(), monitor,
    //    nullptr); m_window_specification.m_height = mode->height; m_window_specification.m_width  = mode->width;
    //    glfwSetWindowUserPointer(m_window_handle, &m_window_specification);
    //}

    // Windowed fullscreen
    // int           count;
    // GLFWmonitor** monitors = glfwGetMonitors(&count);
    // cstring monitor1_name = glfwGetMonitorName(monitors[0]);
    // std::cout << monitor1_name << std::endl;
    // cstring monitor2_name = glfwGetMonitorName(monitors[1]);
    // std::cout << monitor2_name << std::endl;
    // const GLFWvidmode* mode = glfwGetVideoMode(monitors[0]);
    // m_window_handle = glfwCreateWindow(mode->width, mode->height, specification.m_title.c_str(), monitors[0],
    // nullptr);

    GLFWimage images[1]{};
    images[0].pixels = stbi_load("..\\..\\data\\icons\\app_icon.png", &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(m_window_handle, 1, images);
    stbi_image_free(images[0].pixels);

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
                                  sputnik::core::systems::RenderSystem::getInstance()->onWindowResize(width, height);
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
                           switch(action)
                           {
                           case GLFW_PRESS:
                           {
                               window_specification.m_input->UpdateKeyboardState(key, true, true);
                               break;
                           }

                           case GLFW_RELEASE:
                           {
                               window_specification.m_input->UpdateKeyboardState(key, false, false);
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
                                       window_specification.m_input->UpdateMouseButtonState(button, true);
                                       break;
                                   }

                                   case GLFW_RELEASE:
                                   {
                                       window_specification.m_input->UpdateMouseButtonState(button, false);
                                       break;
                                   }
                                   }
                               });

    glfwSetScrollCallback(m_window_handle,
                          [](GLFWwindow* window, double offset_x, double offset_y)
                          {
                              WindowSpecification& window_specification =
                                  *(WindowSpecification*)glfwGetWindowUserPointer(window);
                              window_specification.m_input->UpdateScrollOffset(offset_x, offset_y);
                          });

    glfwSetCursorPosCallback(m_window_handle,
                             [](GLFWwindow* window, double position_x, double position_y)
                             {
                                 WindowSpecification& window_specification =
                                     *(WindowSpecification*)glfwGetWindowUserPointer(window);
                                 window_specification.m_input->UpdateCursorPosition(position_x, position_y);
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

std::pair<u32, u32> Window::GetWindowWidthAndHeight() const
{
    return std::pair<u32, u32>(m_window_specification.m_width, m_window_specification.m_height);
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

void Window::OnUpdate(const sputnik::core::TimeStep& time_step)
{
    glfwPollEvents();
}

void Window::setViewportToCurrentWindowSize()
{
    glfwGetWindowSize(m_window_handle, (int*)&m_window_specification.m_width, (int*)&m_window_specification.m_height);
}

void Window::Shutdown()
{
    glfwDestroyWindow(m_window_handle);
}

} // namespace sputnik::graphics::window