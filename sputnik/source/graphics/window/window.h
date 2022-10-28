#pragma once

#include "core/core.h"

#include <memory>

class GLFWwindow;

namespace sputnik
{

namespace api
{
class GraphicsContext;
}

struct WindowSpecification
{
    unsigned int m_width;
    unsigned int m_height;
    std::string  m_title;
    bool         m_vsync_enabled;
};

class Window
{

public:
    NON_COPYABLE(Window);

    virtual ~Window();

    static Window&       InitializeWindow(const WindowSpecification& specification = WindowSpecification());
    virtual unsigned int GetWidth() const;
    virtual unsigned int GetHeight() const;
    virtual void         SetVsync(bool enable_vsync);
    virtual bool         IsVsyncEnabled();
    virtual GLFWwindow*  GetNativeWindow();
    virtual void         OnUpdate();

protected:
    virtual void Shutdown();

private:
    Window(const WindowSpecification& specification);

protected:
    WindowSpecification                   m_window_specification;
    GLFWwindow*                           m_window_handle;
    std::unique_ptr<api::GraphicsContext> m_graphics_context;
};

} // namespace sputnik