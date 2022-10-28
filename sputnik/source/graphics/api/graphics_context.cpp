#include "pch.h"
#include "graphics_context.h"
#include "graphics/glcore/opengl_context.h"

#include <GLFW/glfw3.h>

namespace sputnik::api
{

GraphicsContext::GraphicsContext(GLFWwindow* window_handle) : m_window_handle(window_handle) {}

GraphicsContext::~GraphicsContext() {}

} // namespace sputnik::api