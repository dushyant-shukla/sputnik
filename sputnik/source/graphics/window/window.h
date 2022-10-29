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
    unsigned int m_width         = 1600;
    unsigned int m_height        = 900;
    std::string  m_title         = "Sputnik";
    bool         m_vsync_enabled = false;
};

class Window
{

public:
    NON_COPYABLE(Window);

    Window(const WindowSpecification& specification = WindowSpecification());
    virtual ~Window();

    virtual unsigned int GetWidth() const;
    virtual unsigned int GetHeight() const;
    virtual void         SetVsync(bool enable_vsync);
    virtual bool         IsVsyncEnabled();
    virtual GLFWwindow*  GetNativeWindow();
    virtual void         OnUpdate();

protected:
    virtual void Shutdown();

private:

protected:
    WindowSpecification                   m_window_specification;
    GLFWwindow*                           m_window_handle;
};

} // namespace sputnik